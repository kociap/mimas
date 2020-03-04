#ifndef MIMAS_MIMAS_INTERNAL_H_INCLUDE
#define MIMAS_MIMAS_INTERNAL_H_INCLUDE

#include <mimas/mimas.h>

typedef enum Mimas_Backend {
    MIMAS_BACKEND_GL,
    MIMAS_BACKEND_VK,
} Mimas_Backend;

typedef struct Mimas_Window_List_Element {
    struct Mimas_Window_List_Element* prev;
    struct Mimas_Window_List_Element* next;
    Mimas_Window* window;
} Mimas_Window_List_Element;

typedef struct Mimas_Internal {
    Mimas_Window* active_window;
    Mimas_Window* clipped_cursor_window;
    Mimas_Window* virtual_cursor_window;
    Mimas_Window_List_Element* window_list;
    Mimas_Display** displays;
    mimas_i64 display_count;
    void* platform;
    Mimas_Backend backend;
    Mimas_Cursor* default_cursor;
} Mimas_Internal;

void _mimas_init_internal(Mimas_Backend);
void _mimas_terminate_internal(void);
mimas_bool _mimas_is_initialized(void);
Mimas_Internal* _mimas_get_mimas_internal(void);

// typedef in mimas/mimas.h
struct Mimas_Display {
    void* native_display;
};

// typedef in mimas/mimas.h
struct Mimas_Window {
    mimas_bool decorated;
    mimas_bool close_requested;
    Mimas_Display* display;
    void* native_window;
    Mimas_Key_Action keys[256];
    mimas_i32 cursor_virtual_pos_x;
    mimas_i32 cursor_virtual_pos_y;
    mimas_bool cursor_clipped;
    Mimas_Rect clip_region;
    mimas_bool cursor_locked;
    Mimas_Cursor* cursor;

    struct {
        mimas_window_activate_callback window_activate;
        void* window_activate_data;
        mimas_window_resize_callback window_resize;
        void* window_resize_data;
        mimas_window_cursor_pos_callback cursor_pos;
        void* cursor_pos_data;
        mimas_window_mouse_button_callback mouse_button;
        void* mouse_button_data;
        mimas_window_key_callback key;
        void* key_data;
        mimas_window_close_requested close_requested;
        void* close_requested_data;
        mimas_window_hittest hittest;
    } callbacks;
};

#endif // !MIMAS_MIMAS_INTERNAL_H_INCLUDE
