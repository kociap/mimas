#include <win/platform.h>
#include <internal.h>
#include <utils.h>
#include <mimas/mimas.h>

#include <stdlib.h>
#include <string.h>


#include <shlobj_core.h>
#include <shlwapi.h>
#include <shobjidl.h>
#include <combaseapi.h>


static IShellItem* create_shell_item_from_path(mimas_char8 const* path) {
    if (!path) { return NULL;  }
    mimas_char16* const wpath = _mimas_utf8_to_utf16(path, -1);
    if (!wpath) {
        return NULL;
    }

    IShellItem* shell_item = NULL;
    SHCreateItemFromParsingName(wpath, NULL, &IID_IShellItem, (void**)&shell_item);

    free(wpath);
    return shell_item;
}

// New implementation using the COM api
mimas_char8* mimas_open_file_dialog(Mimas_File_Dialog_Type type, Mimas_File_Dialog_Flags user_flags, 
                                      Mimas_File_Filter* filters, mimas_u64 filter_count, mimas_char8 const* default_path) {
    IFileDialog* dialog = NULL;
    // Create the file dialog instance. Note that the documentation says to pass the GUID's by reference, but the C API needs pointers.
    CLSID const* cls_id = NULL;
    if (type == MIMAS_FILE_DIALOG_OPEN) {
        cls_id = &CLSID_FileOpenDialog;
    } else if (type == MIMAS_FILE_DIALOG_SAVE) {
        cls_id = &CLSID_FileSaveDialog;
    } else { 
        // User must specify either MIMAS_FILE_DIALOG_OPEN or MIMAS_FILE_DIALOG_SAVE for this function to work.
        return NULL;  
    }

    HRESULT hr = CoCreateInstance(cls_id, NULL, CLSCTX_INPROC_SERVER, &IID_IFileDialog, (void**)&dialog);
    UNUSED(hr);

    if (!dialog) {
        return NULL;
    }
    
    // Set options for the file dialog
    DWORD flags;
    dialog->lpVtbl->GetOptions(dialog, &flags);
    if (user_flags & MIMAS_FILE_DIALOG_PICK_FILES) { flags |= FOS_FORCEFILESYSTEM; } 
    if (user_flags & MIMAS_FILE_DIALOG_PICK_FOLDERS) { flags |= FOS_PICKFOLDERS; }
    // We don't want the file dialog changing the current working directory
    flags |= FOS_NOCHANGEDIR;
    dialog->lpVtbl->SetOptions(dialog, flags);

    IShellItem* default_folder = NULL;
    if (default_path) {
        default_folder = create_shell_item_from_path(default_path);
    }
    if (default_folder) {
        dialog->lpVtbl->SetFolder(dialog, default_folder);
    }

    // Set file filters
    COMDLG_FILTERSPEC* win_filters = NULL;
    wchar_t** filter_strings = NULL;
    if (filters != NULL) {
        win_filters = malloc(filter_count * sizeof(COMDLG_FILTERSPEC));
        filter_strings = malloc(2 * filter_count * sizeof(wchar_t*));
        // Fill win_filters array with the correct filters. For this we have to convert the data to wide strings
        for (mimas_u64 i = 0; i < filter_count; ++i) {
            mimas_char16* const wname = _mimas_utf8_to_utf16(filters[i].name, -1);
            mimas_char16* const wfilter = _mimas_utf8_to_utf16(filters[i].filter, -1);
            // Assign strings to our allocated arrays
            filter_strings[2 * i] = wname;
            filter_strings[2 * i + 1] = wfilter;
            win_filters[i].pszName = wname;
            win_filters[i].pszSpec = wfilter;
        }
        dialog->lpVtbl->SetFileTypes(dialog, filter_count, win_filters);
    }

    dialog->lpVtbl->Show(dialog, NULL);

    // Reinstall input listener. This is because creating a file dialog window somehow unhooks our global input event listener.
    _mimas_uninstall_input_listener();
    _mimas_install_input_listener();

    // Retrieve result
    IShellItem* result = NULL;
    dialog->lpVtbl->GetResult(dialog, &result);
    if (!result) { 
        dialog->lpVtbl->Release(dialog);
        if (default_folder) { default_folder->lpVtbl->Release(default_folder); }
        free(win_filters);
        if (filter_strings) {
            for (mimas_u64 i = 0; i < 2 * filter_count; ++i) {
                free(filter_strings[i]);
            }
            free(filter_strings);
        }
        return NULL;  
    }

    PWSTR result_path = NULL;
    result->lpVtbl->GetDisplayName(result, SIGDN_FILESYSPATH, &result_path);
    if (!result_path) { 
        result->lpVtbl->Release(result);
        dialog->lpVtbl->Release(dialog);
        if (default_folder) { default_folder->lpVtbl->Release(default_folder); }
        free(win_filters);
        if (filter_strings) {
            for (mimas_u64 i = 0; i < 2 * filter_count; ++i) {
                free(filter_strings[i]);
            }
            free(filter_strings);
        }
        return NULL;  
    }

    mimas_i64 const mb_buffer_size = WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, result_path, -1, NULL, 0, NULL, NULL);
    mimas_char8* buffer = malloc(mb_buffer_size);
    WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, result_path, -1, (char*)buffer, mb_buffer_size, NULL, NULL);
    
    CoTaskMemFree(result_path);
    result->lpVtbl->Release(result);
    dialog->lpVtbl->Release(dialog);
    if (default_folder) { default_folder->lpVtbl->Release(default_folder); }
    free(win_filters);
    if (filter_strings) {
        for (mimas_u64 i = 0; i < 2 * filter_count; ++i) {
            free(filter_strings[i]);
        }
        free(filter_strings);
    }

    return buffer;
}
