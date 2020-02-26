#ifndef MIMAS_WIN_PLATFORM_H_INCLUDE
#define MIMAS_WIN_PLATFORM_H_INCLUDE

#define NOMINMAX 1
#define WIN32_LEAN_AND_MEAN 1

#define _UNICODE 1
#define UNICODE 1

#include <Windows.h>
#include <Windowsx.h>
#include <dwmapi.h>

#include <mimas/mimas.h>

#define MIMAS_WINDOW_CLASS_NAME L"anton.mimas.mingwlul.window"

typedef struct Mimas_Win_Window {
    HWND handle;
    HDC hdc;
    mimas_bool non_client_activate;
} Mimas_Win_Window;

typedef struct Mimas_Win_Display {
    HMONITOR hmonitor;
    char display_name[64];
    char adapter_name[64];
} Mimas_Win_Display;

typedef struct Mimas_Win_Platform {
    Mimas_Key_Action key_state[256];
    Mimas_Window* dummy_window;
} Mimas_Win_Platform;

Mimas_Display** _mimas_get_connected_displays(mimas_i64* const count);
void _mimas_free_displays(Mimas_Display** displays, mimas_i64 const count);

#endif // !MIMAS_WIN_PLATFORM_H_INCLUDE
