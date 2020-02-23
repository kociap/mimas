#include <linux/x11/init.h>
#include <linux/x11/platform.h>

#include <platform.h>
#include <internal.h>
#include <platform_gl.h>
#include <platform_vk.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void mimas_terminate() {
    Mimas_Internal* const _mimas = _mimas_get_mimas_internal();
    mimas_platform_terminate(_mimas->backend);
    _mimas_terminate_internal();
}

mimas_bool mimas_platform_init(Mimas_Backend backend) {
    if (!mimas_load_x11()) {
        return mimas_false;
    }

    Mimas_Internal* const _mimas = _mimas_get_mimas_internal();
    _mimas->backend = backend;

    if (_mimas->backend == MIMAS_BACKEND_GL) {
        //!TODO
    } else {
        mimas_platform_init_vk_backend();
    }

    return mimas_true;
}

Mimas_Window* mimas_platform_create_window(Mimas_Window_Create_Info info) {
    Mimas_Window* const window = (Mimas_Window*)malloc(sizeof(Mimas_Window));
    memset(window, 0, sizeof(Mimas_Window));

    window->decorated = info.decorated;

    Mimas_X11_Window* const native_window = (Mimas_X11_Window*)malloc(sizeof(Mimas_X11_Window));
    memset(window, 0, sizeof(Mimas_Window));

    native_window->display = mimas_XOpenDisplay(NULL);

    int default_screen = DefaultScreen(native_window->display);
    Screen* screen = mimas_XScreenOfDisplay(native_window->display, default_screen);

    native_window->window =
        mimas_XCreateSimpleWindow(
            native_window->display,
            screen->root,
            0, 0, info.width, info.height, 1,
            screen->black_pixel,
            screen->white_pixel);

    mimas_XMapWindow(native_window->display, native_window->window);
    mimas_XStoreName(native_window->display, native_window->window, info.title);
    mimas_XSetIconName(native_window->display, native_window->window, info.title);

    window->native_window = window;

    return window;
}