#include <mimas/mimas.h>
#include <internal.h>
#include <platform.h>
#include <platform_gl.h>
#include <platform_vk.h>

#include <stdlib.h>
#include <string.h>

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

void mimas_poll_events(void) {
    mimas_platform_poll_events();
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

Mimas_Display_Settings mimas_get_display_settings(Mimas_Display* display) {
    return mimas_platform_get_display_settings(display);
}

void mimas_fullscreen_window(Mimas_Window* window, Mimas_Display* display) {
    mimas_platform_fullscreen_window(window, display);
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

void mimas_set_window_hittest(Mimas_Window* window, mimas_window_hittest callback) {
    window->callbacks.hittest = callback;
}

Mimas_Callback mimas_get_window_hittest(Mimas_Window* window) {
    return (Mimas_Callback){(void*)window->callbacks.hittest, NULL};
}

void mimas_set_window_pos(Mimas_Window* const window, mimas_i32 const x, mimas_i32 const y) {
    mimas_platform_set_window_pos(window, x, y);
}

void mimas_get_window_pos(Mimas_Window* const window, mimas_i32* const x, mimas_i32* const y) {
    mimas_platform_get_window_pos(window, x, y);
}

void mimas_set_window_content_pos(Mimas_Window* const window, mimas_i32 const x, mimas_i32 const y) {
    mimas_platform_set_window_content_pos(window, x, y);
}

void mimas_get_window_content_pos(Mimas_Window* const window, mimas_i32* const x, mimas_i32* const y) {
    mimas_platform_get_window_content_pos(window, x, y);
}

void mimas_set_window_content_size(Mimas_Window* const window, mimas_i32 const width, mimas_i32 const height) {
    mimas_platform_set_window_content_size(window, width, height);
}

void mimas_get_window_content_size(Mimas_Window* const window, mimas_i32* const width, mimas_i32* const height) {
    mimas_platform_get_window_content_size(window, width, height);
}

mimas_bool mimas_is_window_active(Mimas_Window* window) {
    Mimas_Internal* const _mimas = _mimas_get_mimas_internal();
    return _mimas->active_window == window;
}

void mimas_show_window(Mimas_Window* const window) {
    mimas_platform_show_window(window);
}

void mimas_hide_window(Mimas_Window* const window) {
    mimas_platform_hide_window(window);
}

void mimas_restore_window(Mimas_Window* const window) {
    mimas_platform_restore_window(window);
}

void mimas_minimize_window(Mimas_Window* const window) {
    mimas_platform_minimize_window(window);
}

void mimas_maximize_window(Mimas_Window* const window) {
    mimas_platform_maximize_window(window);
}

Mimas_Key_Action mimas_get_key(Mimas_Key const key) {
    return mimas_platform_get_key(key);
}

void mimas_clip_cursor(Mimas_Window* const window, Mimas_Rect const* const region) {
    mimas_platform_clip_cursor(window, region);
}

void mimas_lock_cursor(Mimas_Window* const window) {
    mimas_platform_lock_cursor(window);
}

void mimas_unlock_cursor(Mimas_Window* const window) {
    mimas_platform_unlock_cursor(window);
}

void mimas_set_cursor_pos(mimas_i32 const x, mimas_i32 const y) {
    mimas_platform_set_cursor_pos(x, y);
}

void mimas_get_cursor_pos(mimas_i32* const x, mimas_i32* const y) {
    mimas_platform_get_cursor_pos(x, y);
}

Mimas_Cursor* mimas_create_standard_cursor(Mimas_Standard_Cursor cursor) {
    return mimas_platform_create_standard_cursor(cursor);
}

void mimas_destroy_cursor(Mimas_Cursor* cursor) {
    mimas_platform_destroy_cursor(cursor);
}

void mimas_set_cursor(Mimas_Window* window, Mimas_Cursor* cursor) {
    mimas_platform_set_cursor(window, cursor);
}

Mimas_System_Time mimas_get_utc_system_time(void) {
    return mimas_platform_get_utc_system_time();
}

Mimas_System_Time mimas_get_local_system_time(void) {
    return mimas_platform_get_local_system_time();
}

double mimas_get_time(void) {
    return mimas_platform_get_time();
}
