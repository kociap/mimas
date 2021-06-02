#include <mimas/mimas.h>
#include <internal.h>
#include <platform.h>
#include <platform_gl.h>
#include <platform_vk.h>

#include <stdlib.h>
#include <string.h>

mimas_i64 mimas_string_size_bytes(Mimas_String const string) {
    return string.end - string.begin;
}

void mimas_free_string(Mimas_String const string) {
    free(string.begin);
}

mimas_i64 mimas_string_view_size_bytes(Mimas_String_View const string) {
    return string.end - string.begin;
}

void mimas_terminate(void) {
    Mimas_Internal* const _mimas = _mimas_get_mimas_internal();
    for(Mimas_Window_List_Element* elem = _mimas->window_list; elem != NULL;) {
        mimas_platform_destroy_window(elem->window);
        free(elem);
        elem = elem->next;
    }
    _mimas->window_list = NULL;
    mimas_platform_terminate(_mimas->backend);
    _mimas_terminate_internal();
}

Mimas_Display* mimas_get_primary_display(void) {
    Mimas_Internal* const _mimas = _mimas_get_mimas_internal();
    return _mimas->displays[0];
}

Mimas_Display** mimas_get_displays(mimas_i64* count) {
    Mimas_Internal* const _mimas = _mimas_get_mimas_internal();
    *count = _mimas->display_count;
    return _mimas->displays;
}

Mimas_Window* mimas_create_window(Mimas_Window_Create_Info const info) {
    Mimas_Window_List_Element* const elem = (Mimas_Window_List_Element*)malloc(sizeof(Mimas_Window_List_Element));
    if(!elem) {
        // TODO: Error
        return NULL;
    }
    memset(elem, 0, sizeof(Mimas_Window_List_Element));

    Mimas_Window* window = mimas_platform_create_window(info);
    if(!window) {
        free(elem);
        // TODO: Error
        return NULL;
    }

    Mimas_Internal* const _mimas = _mimas_get_mimas_internal();
    elem->window = window;
    if(_mimas->window_list) {
        Mimas_Window_List_Element* const next_elem = _mimas->window_list;
        next_elem->prev = elem;
        elem->next = next_elem;
        _mimas->window_list = elem;
    } else {
        _mimas->window_list = elem;
    }

    return window;
}

void mimas_destroy_window(Mimas_Window* window) {
    mimas_platform_destroy_window(window);
    Mimas_Internal* const _mimas = _mimas_get_mimas_internal();
    Mimas_Window_List_Element* elem = _mimas->window_list;
    while(elem->window != window) {
        elem = elem->next;
    }

    if(elem->prev || elem->next) {
        if(elem->prev) {
            elem->prev->next = elem->next;
        } else {
            // elem is first in the list. We have to reassign the head of the list
            _mimas->window_list = elem->next;
        }

        if(elem->next) {
            elem->next->prev = elem->prev;
        }
    } else {
        _mimas->window_list = NULL;
    }

    free(elem);
}

mimas_bool mimas_close_requested(Mimas_Window* window) {
    return window->close_requested;
}

void mimas_set_window_activate_callback(Mimas_Window* window, mimas_window_activate_callback callback, void* user_data) {
    window->callbacks.window_activate = callback;
    window->callbacks.window_activate_data = user_data;
}

Mimas_Callback mimas_get_window_activate_callback(Mimas_Window* window) {
    return (Mimas_Callback){(void*)window->callbacks.window_activate, window->callbacks.window_activate_data};
}

void mimas_set_window_resize_callback(Mimas_Window* window, mimas_window_resize_callback callback, void* user_data) {
    window->callbacks.window_resize = callback;
    window->callbacks.window_resize_data = user_data;
}

Mimas_Callback mimas_get_window_resize_callback(Mimas_Window* window) {
    return (Mimas_Callback){(void*)window->callbacks.window_resize, window->callbacks.window_resize_data};
}

void mimas_set_window_cursor_pos_callback(Mimas_Window* window, mimas_window_cursor_pos_callback callback, void* user_data) {
    window->callbacks.cursor_pos = callback;
    window->callbacks.cursor_pos_data = user_data;
}

Mimas_Callback mimas_get_window_cursor_pos_callback(Mimas_Window* window) {
    return (Mimas_Callback){(void*)window->callbacks.cursor_pos, window->callbacks.cursor_pos_data};
}

void mimas_set_window_mouse_button_callback(Mimas_Window* window, mimas_window_mouse_button_callback callback, void* user_data) {
    window->callbacks.mouse_button = callback;
    window->callbacks.mouse_button_data = user_data;
}

Mimas_Callback mimas_get_window_mouse_button_callback(Mimas_Window* window) {
    return (Mimas_Callback){(void*)window->callbacks.mouse_button, window->callbacks.mouse_button_data};
}

MIMAS_API void mimas_set_window_scroll_callback(Mimas_Window* window, mimas_window_scroll_callback callback, void* user_data) {
    window->callbacks.scroll = callback;
    window->callbacks.scroll_data = user_data;
}

MIMAS_API Mimas_Callback mimas_get_window_scroll_callback(Mimas_Window* window) {
    return (Mimas_Callback){(void*)window->callbacks.scroll, window->callbacks.scroll_data};
}

void mimas_set_window_key_callback(Mimas_Window* window, mimas_window_key_callback callback, void* user_data) {
    window->callbacks.key = callback;
    window->callbacks.key_data = user_data;
}

Mimas_Callback mimas_get_window_key_callback(Mimas_Window* window) {
    return (Mimas_Callback){(void*)window->callbacks.key, window->callbacks.key_data};
}

void mimas_set_window_char_callback(Mimas_Window* window, mimas_window_char_callback callback, void* user_data) {
    window->callbacks.character = callback;
    window->callbacks.character_data = user_data;
}

Mimas_Callback mimas_get_window_char_callback(Mimas_Window* window) {
    return (Mimas_Callback){(void*)window->callbacks.character, window->callbacks.character_data};
}

void mimas_set_window_hittest(Mimas_Window* window, mimas_window_hittest callback) {
    window->callbacks.hittest = callback;
}

Mimas_Callback mimas_get_window_hittest(Mimas_Window* window) {
    return (Mimas_Callback){(void*)window->callbacks.hittest, NULL};
}

mimas_bool mimas_is_window_active(Mimas_Window* window) {
    Mimas_Internal* const _mimas = _mimas_get_mimas_internal();
    return _mimas->active_window == window;
}
