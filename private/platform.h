#ifndef MIMAS_MIMAS_PLATFORM_H_INCLUDE
#define MIMAS_MIMAS_PLATFORM_H_INCLUDE

#include <internal.h>
#include <mimas/mimas.h>

void mimas_platform_terminate();

void mimas_platform_poll_events();

Mimas_Window* mimas_platform_create_window(mimas_i32 width, mimas_i32 height, char const* title, mimas_bool decorated);
void mimas_platform_destroy_window(Mimas_Window*);

void mimas_platform_set_window_pos(Mimas_Window*, mimas_i32 x, mimas_i32 y);
void mimas_platform_get_window_pos(Mimas_Window*, mimas_i32* x, mimas_i32* y);
void mimas_platform_set_window_content_pos(Mimas_Window*, mimas_i32 x, mimas_i32 y);
void mimas_platform_get_window_content_pos(Mimas_Window*, mimas_i32* x, mimas_i32* y);
void mimas_platform_set_window_content_size(Mimas_Window*, mimas_i32 width, mimas_i32 height);
void mimas_platform_get_window_content_size(Mimas_Window*, mimas_i32* width, mimas_i32* height);

void mimas_platform_show_window(Mimas_Window*);
void mimas_platform_hide_window(Mimas_Window*);
void mimas_platform_restore_window(Mimas_Window*);
void mimas_platform_minimize_window(Mimas_Window*);
void mimas_platform_maximize_window(Mimas_Window*);

#endif // !MIMAS_MIMAS_PLATFORM_H_INCLUDE
