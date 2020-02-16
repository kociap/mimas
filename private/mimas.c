#include <mimas/mimas.h>
#include <internal.h>
#include <platform.h>

void mimas_terminate() {
    mimas_platform_terminate();
    _mimas_terminate_internal();
}

void mimas_poll_events() {
    mimas_platform_poll_events();
}

Mimas_Window* mimas_create_window(mimas_i32 width, mimas_i32 height, char const* title) {
    return mimas_platform_create_window(width, height, title, mimas_false);
}

void mimas_destroy_window(Mimas_Window* window) {
    mimas_platform_destroy_window(window);
}

void mimas_set_window_activate_callback(Mimas_Window* window, mimas_window_activate_callback callback, void* user_data) {
    window->callbacks.window_activate = callback;
    window->callbacks.window_activate_data = user_data;
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
