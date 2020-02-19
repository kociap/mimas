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

typedef struct {
    HWND handle;
} Mimas_Win_Window;

typedef struct {
    mimas_u8 keyboard_state[256];
    mimas_i32 virtaul_keys[256];
    Mimas_Key keys[256];
    Mimas_Window* dummy_window;
} Mimas_Win_Platform;

#endif // !MIMAS_WIN_PLATFORM_H_INCLUDE
