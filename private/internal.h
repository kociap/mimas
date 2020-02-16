#ifndef MIMAS_MIMAS_INTERNAL_H_INCLUDE
#define MIMAS_MIMAS_INTERNAL_H_INCLUDE

#include <mimas/mimas.h>

typedef enum {
    MIMAS_BACKEND_GL,
    MIMAS_BACKEND_VK,
} Mimas_Backend;

typedef struct {
    void* platform;
    Mimas_Backend backend;
} Mimas_Internal;

void _mimas_init_internal(Mimas_Backend);
void _mimas_terminate_internal();
mimas_bool _mimas_is_initialized();
Mimas_Internal* _mimas_get_mimas_internal();

// typedef in mimas/mimas.h
struct Mimas_Window {
    mimas_bool decorated;
    void* native_window;

    struct {
        mimas_window_activate_callback window_activate;
        void* window_activate_data;
    } callbacks;
};

#endif // !MIMAS_MIMAS_INTERNAL_H_INCLUDE
