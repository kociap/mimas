#ifndef MIMAS_MIMAS_PLATFORM_H_INCLUDE
#define MIMAS_MIMAS_PLATFORM_H_INCLUDE

#include <internal.h>
#include <mimas/mimas.h>

mimas_bool mimas_platform_init(Mimas_Backend backend);
void mimas_platform_terminate(Mimas_Backend backend);

void mimas_platform_poll_events(void);

Mimas_Display_Settings mimas_platform_get_display_settings(Mimas_Display* display);
void mimas_platform_fullscreen_window(Mimas_Window* window, Mimas_Display* display);

Mimas_Window* mimas_platform_create_window(Mimas_Window_Create_Info);
void mimas_platform_destroy_window(Mimas_Window* window);

void mimas_platform_set_window_pos(Mimas_Window* window, mimas_i32 x, mimas_i32 y);
void mimas_platform_get_window_pos(Mimas_Window* window, mimas_i32* x, mimas_i32* y);
void mimas_platform_set_window_content_pos(Mimas_Window* window, mimas_i32 x, mimas_i32 y);
void mimas_platform_get_window_content_pos(Mimas_Window* window, mimas_i32* x, mimas_i32* y);
void mimas_platform_set_window_content_size(Mimas_Window* window, mimas_i32 width, mimas_i32 height);
void mimas_platform_get_window_content_size(Mimas_Window* window, mimas_i32* width, mimas_i32* height);

void mimas_platform_show_window(Mimas_Window* window);
void mimas_platform_hide_window(Mimas_Window* window);
void mimas_platform_restore_window(Mimas_Window* window);
void mimas_platform_minimize_window(Mimas_Window* window);
void mimas_platform_maximize_window(Mimas_Window* window);

Mimas_Key_Action mimas_platform_get_key(Mimas_Key button);

void mimas_platform_clip_cursor(Mimas_Window* window, Mimas_Rect const* region);
void mimas_platform_lock_cursor(Mimas_Window* const window);
void mimas_platform_unlock_cursor(Mimas_Window* const window);
void mimas_platform_set_cursor_pos(mimas_i32 const x, mimas_i32 const y);
void mimas_platform_get_cursor_pos(mimas_i32* x, mimas_i32* y);
Mimas_Cursor* mimas_platform_create_standard_cursor(Mimas_Standard_Cursor cursor);
void mimas_platform_destroy_cursor(Mimas_Cursor* cursor);
void mimas_platform_set_cursor(Mimas_Window* window, Mimas_Cursor* cursor);


#endif // !MIMAS_MIMAS_PLATFORM_H_INCLUDE
