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


static IShellItem* create_shell_item_from_path(char const* path) {
    if (!path) { return NULL;  }
    // Get wide version of the path
    int const wpath_buffer_size = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, path, -1, NULL, 0);
    wchar_t* wpath = malloc(sizeof(wchar_t) * wpath_buffer_size);
    if (!wpath) { return NULL; }
    MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, path, -1, wpath, wpath_buffer_size);

    IShellItem* shell_item = NULL;
    SHCreateItemFromParsingName(wpath, NULL, &IID_IShellItem, (void**)&shell_item);

    free(wpath);
    return shell_item;
}

// New implementation using the COM api
char* mimas_platform_open_file_dialog(Mimas_File_Dialog_Type type, Mimas_File_Dialog_Flags user_flags, 
                                      Mimas_File_Filter* filters, mimas_u64 filter_count, char const* default_path) {
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
            // Filter name
            int const wname_buffer_size = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, filters[i].name, -1, NULL, 0);
            wchar_t* wname = malloc(sizeof(wchar_t) * wname_buffer_size);
            MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, filters[i].name, -1, wname, wname_buffer_size);
            // Filter string
            int const wfilter_buffer_size = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, filters[i].filter, -1, NULL, 0);
            wchar_t* wfilter = malloc(sizeof(wchar_t) * wfilter_buffer_size);
            MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, filters[i].filter, -1, wfilter, wfilter_buffer_size);
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

    int const mb_buffer_size = WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, result_path, -1, NULL, 0, NULL, NULL);
    char* buffer = malloc(mb_buffer_size);
    WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, result_path, -1, buffer, mb_buffer_size, NULL, NULL);
    
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
