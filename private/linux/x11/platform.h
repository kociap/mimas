#ifndef MIMAS_PLATFORM_H
#define MIMAS_PLATFORM_H

#include <mimas/mimas.h>
#include <X11/Xlib.h>

typedef struct {
    Window window;
    Display* display;
} Mimas_X11_Window;

typedef struct {
    Mimas_Window* dummy_window;
} Mimas_X11_Platform;

#endif //MIMAS_PLATFORM_H
