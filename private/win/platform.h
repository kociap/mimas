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
    // Used by the event loop to determine whether the window was activated
    // by an action in the client area for cursor clipping/locking. 
    mimas_bool non_client_activate;
    // Used by WM_CHAR for translating 2 consecutive WM_CHAR messages into a single codepoint.
    mimas_char32 cooked_character;
    mimas_bool cooking_character;
} Mimas_Win_Window;

typedef struct Mimas_Win_Display {
    HMONITOR hmonitor;
    // 64 because the names might be at most 32 wchars (enforced by winapi).
    mimas_char8 display_name[64];
    mimas_char8 adapter_name[64];
} Mimas_Win_Display;

typedef struct Mimas_Win_Platform {
    Mimas_Key_Action key_state[256];
    Mimas_Window* dummy_window;
    // Whether the input events will be generated while the application is out of focus
    mimas_bool enable_raw_input_sink;
} Mimas_Win_Platform;

Mimas_Display** _mimas_get_connected_displays(mimas_i64* const count);
void _mimas_free_displays(Mimas_Display** displays, mimas_i64 const count);

// _mimas_install_input_listener
// Installs the global raw input listener 
//
void _mimas_install_input_listener();

void _mimas_uninstall_input_listener();

#endif // !MIMAS_WIN_PLATFORM_H_INCLUDE
