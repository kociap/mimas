#include <mimas/mimas.h>
#include <internal.h>
#include <platform.h>
#include <platform_gl.h>
#include <platform_vk.h>

#include <stdlib.h>

void mimas_terminate() {
    Mimas_Internal* const _mimas = _mimas_get_mimas_internal();
    mimas_platform_terminate(_mimas->backend);
    _mimas_terminate_internal();
}

void mimas_poll_events() {
    mimas_platform_poll_events();
}

Mimas_Window* mimas_create_window(Mimas_Window_Create_Info const info) {
    return mimas_platform_create_window(info);
}

void mimas_destroy_window(Mimas_Window* window) {
    mimas_platform_destroy_window(window);
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

void mimas_set_window_mouse_button_callback(Mimas_Window* window, mimas_window_cursor_pos_callback callback, void* user_data) {
    window->callbacks.mouse_button = callback;
    window->callbacks.mouse_button_data = user_data;
}

Mimas_Callback mimas_get_window_mouse_button_callback(Mimas_Window* window) {
    return (Mimas_Callback){(void*)window->callbacks.mouse_button, window->callbacks.mouse_button_data};
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

void mimas_swap_buffers(Mimas_Window* const window) {
    mimas_platform_swap_buffers(window);
}

void mimas_set_swap_interval(mimas_i32 const interval) {
    mimas_platform_set_swap_interval(interval);
}

mimas_i32 mimas_get_swap_interval() {
    return mimas_platform_get_swap_interval();
}

void mimas_set_cursor_mode(Mimas_Window* const window, Mimas_Cursor_Mode const cursor_mode) {
    mimas_platform_set_cursor_mode(window, cursor_mode);
}

void mimas_get_cursor_pos(mimas_i32* const x, mimas_i32* const y) {
    mimas_platform_get_cursor_pos(x, y);
}

Mimas_Key_Action mimas_get_key(Mimas_Key const key) {
    return mimas_platform_get_key(key);
}
