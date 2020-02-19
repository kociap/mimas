#ifndef MIMAS_MIMAS_INTERNAL_H_INCLUDE
#define MIMAS_MIMAS_INTERNAL_H_INCLUDE

#include <mimas/mimas.h>

typedef enum Mimas_Backend {
    MIMAS_BACKEND_GL,
    MIMAS_BACKEND_VK,
} Mimas_Backend;

typedef struct Mimas_Internal {
    void* platform;
    Mimas_Backend backend;
    Mimas_Window* active_window;
} Mimas_Internal;

void _mimas_init_internal(Mimas_Backend);
void _mimas_terminate_internal();
mimas_bool _mimas_is_initialized();
Mimas_Internal* _mimas_get_mimas_internal();

// typedef in mimas/mimas.h
struct Mimas_Window {
    mimas_bool decorated;
    mimas_bool close_requested;
    Mimas_Cursor_Mode cursor_mode;
    void* native_window;
    Mimas_Key_Action keys[256];

    struct {
        mimas_window_activate_callback window_activate;
        void* window_activate_data;
        mimas_window_cursor_pos_callback cursor_pos;
        void* cursor_pos_data;
        mimas_window_key_callback key;
        void* key_data;
        mimas_window_hittest hittest;
    } callbacks;
};

#endif // !MIMAS_MIMAS_INTERNAL_H_INCLUDE
