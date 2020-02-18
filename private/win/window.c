#include <win/platform.h>

#include <platform.h>
#include <internal.h>
#include <win/wgl.h>

#include <wingdi.h>

#include <stdlib.h>
#include <string.h>

#include <stdio.h>

static mimas_u32 get_window_styles(Mimas_Window const* const window) {
    return WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP | WS_THICKFRAME | WS_CAPTION | WS_SYSMENU | WS_MAXIMIZEBOX | WS_MINIMIZEBOX;
}

static mimas_u32 get_window_extended_styles(Mimas_Window const* const window) {
    return WS_EX_APPWINDOW;
}

// Capture cursor in the client area of window.
//
static void capture_cursor(Mimas_Window* const window) {
    Mimas_Win_Window* const native_window = (Mimas_Win_Window*)window->native_window;
    RECT clientRect;
    GetClientRect(native_window, &clientRect);
    ClientToScreen(native_window, (POINT*)&clientRect.left);
    ClientToScreen(native_window, (POINT*)&clientRect.right);
    ClipCursor(&clientRect);
}

static void release_captured_cursor() {
    ClipCursor(NULL);
}

static void enable_virtual_cursor(Mimas_Window* const window) {
    // Capture in case we lag and the cursor moves out of the window.
    capture_cursor(window);

}

static void disable_virtual_cursor(Mimas_Window* const window) {
    release_captured_cursor(window);
}

static mimas_i32 window_hit_test(mimas_i32 const cursor_x, mimas_i32 const cursor_y, RECT const window_rect) {
    enum Region_Mask {
        client = 0, 
        left = 1, 
        right = 2, 
        top = 4, 
        bottom = 8
    };

    mimas_i32 const border_width = 8;

    mimas_u32 result = 0;
    // printf("Hittest cursor: %d %d\n", cursor_x, cursor_y);
    result |= left * (cursor_x < window_rect.left + border_width);
    result |= right * (cursor_x >= window_rect.right - border_width);
    result |= top * (cursor_y < window_rect.top + border_width);
    result |= bottom * (cursor_y >= window_rect.bottom - border_width);

    switch(result) {
        case left          : return HTLEFT;
        case right         : return HTRIGHT;
        case top           : return HTTOP;
        case bottom        : return HTBOTTOM;
        case top | left    : return HTTOPLEFT;
        case top | right   : return HTTOPRIGHT;
        case bottom | left : return HTBOTTOMLEFT;
        case bottom | right: return HTBOTTOMRIGHT;
        case client        : return HTCAPTION;
        default            : return HTNOWHERE;
    }
}

static LRESULT window_proc(HWND const hwnd, UINT const msg, WPARAM const wparam, LPARAM const lparam) {
    Mimas_Window* const window = GetPropW(hwnd, L"Mimas_Window");
    switch(msg) {
        case WM_ACTIVATE: {
            if(!window->decorated) {
                MARGINS const margins = {1, 1, 1, 1};
                DwmExtendFrameIntoClientArea(hwnd, &margins);
                SetWindowPos(hwnd, NULL, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER);
            }

            LRESULT const res = DefWindowProc(hwnd, msg, wparam, lparam);
            if(window->callbacks.window_activate) {
                window->callbacks.window_activate(window, wparam != 0, window->callbacks.window_activate_data);
            }
            return res;
        } break;

        case WM_NCCALCSIZE: {
            if(wparam == TRUE && !window->decorated) {
                return 0;
            }
        } break;

        case WM_NCHITTEST: {
            if(!window->decorated) {
                RECT window_rect;
                GetWindowRect(hwnd, &window_rect);
                return window_hit_test(GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam), window_rect);
            }
        } break;
    }

    return DefWindowProc(hwnd, msg, wparam, lparam);
}

// Returns 1 if succeeded, 0 otherwise
static mimas_bool register_window_class() {
    WNDCLASSEX wndclass = {
        .cbSize = sizeof(WNDCLASSEX),
        .style = CS_HREDRAW | CS_VREDRAW,
        .lpfnWndProc = window_proc,
        .hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH),
        .hCursor = LoadCursor(NULL, IDC_ARROW),
        .lpszClassName = MIMAS_WINDOW_CLASS_NAME,
    };
    return RegisterClassEx(&wndclass);
}

// Returns 1 if succeeded, 0 otherwise.
static mimas_bool unregister_window_class() {
    return UnregisterClass(MIMAS_WINDOW_CLASS_NAME, NULL);
}

static Mimas_Window* create_native_window(Mimas_Window_Create_Info const info) {
    Mimas_Window* const window = (Mimas_Window*)malloc(sizeof(Mimas_Window));
    memset(window, 0, sizeof(Mimas_Window));

    window->decorated = info.decorated;

    mimas_u32 const style = WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP | WS_THICKFRAME | WS_CAPTION | WS_SYSMENU | WS_MAXIMIZEBOX | WS_MINIMIZEBOX;
    int const wtitle_buffer_size = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, info.title, -1, NULL, 0);
    wchar_t* wtitle = malloc(sizeof(wchar_t) * wtitle_buffer_size);
    MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, info.title, -1, wtitle, wtitle_buffer_size);
    HWND const hwnd = CreateWindowEx(WS_EX_APPWINDOW, MIMAS_WINDOW_CLASS_NAME, wtitle, style, CW_USEDEFAULT, CW_USEDEFAULT, info.width, info.height, NULL, NULL, GetModuleHandle(NULL), NULL);
    free(wtitle);

    if(!hwnd) {
        free(window);
        // TODO: Error
        return NULL;
    }

    HDC const hdc = GetDC(hwnd);
    PIXELFORMATDESCRIPTOR pfd = {
        .nSize = sizeof(PIXELFORMATDESCRIPTOR),
        .nVersion = 1,
        .dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
        .iPixelType = PFD_TYPE_RGBA,
        .cColorBits = 24,
        .cRedBits = 8,
        .cGreenBits = 8,
        .cBlueBits = 8,
        .cAlphaBits = 8,
        .cDepthBits = 24,
        .cStencilBits = 8,
    };
    int const pixf = ChoosePixelFormat(hdc, &pfd);
    if(!SetPixelFormat(hdc, pixf, &pfd)) {
        DestroyWindow(hwnd);
        free(window);
        // TODO: Error
        return NULL;
    }

    Mimas_Win_Window* native_window = (Mimas_Win_Window*)malloc(sizeof(Mimas_Win_Window));
    native_window->handle = hwnd;
    window->native_window = native_window;

    SetProp(hwnd, L"Mimas_Window", (HANDLE)window);

    if(!info.decorated) {
        MARGINS const margins = {1, 1, 1, 1};
        DwmExtendFrameIntoClientArea(hwnd, &margins);
        SetWindowPos(hwnd, NULL, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER);
    }

    return window;
}

static void destroy_native_window(Mimas_Window* const window) {
    Mimas_Win_Window* const native_window = (Mimas_Win_Window*)window->native_window;
    DestroyWindow(native_window->handle);
    free(native_window);
    free(window);
}

mimas_bool mimas_platform_init_with_gl() {
    Mimas_Win_Platform* const platform = (Mimas_Win_Platform*)malloc(sizeof(Mimas_Win_Platform));
    memset(platform, 0, sizeof(Mimas_Win_Platform));
    memset(platform->keys, -1, sizeof(platform->keys));

    platform->keys[0x30] = MIMAS_KEY_0;
    platform->keys[0x31] = MIMAS_KEY_1;
    platform->keys[0x32] = MIMAS_KEY_2;
    platform->keys[0x33] = MIMAS_KEY_3;
    platform->keys[0x34] = MIMAS_KEY_4;
    platform->keys[0x35] = MIMAS_KEY_5;
    platform->keys[0x36] = MIMAS_KEY_6;
    platform->keys[0x37] = MIMAS_KEY_7;
    platform->keys[0x38] = MIMAS_KEY_8;
    platform->keys[0x39] = MIMAS_KEY_9;

    platform->keys[0x41] = MIMAS_KEY_A;
    platform->keys[0x42] = MIMAS_KEY_B;
    platform->keys[0x43] = MIMAS_KEY_C;
    platform->keys[0x44] = MIMAS_KEY_D;
    platform->keys[0x45] = MIMAS_KEY_E;
    platform->keys[0x46] = MIMAS_KEY_F;
    platform->keys[0x47] = MIMAS_KEY_G;
    platform->keys[0x48] = MIMAS_KEY_H;
    platform->keys[0x49] = MIMAS_KEY_I;
    platform->keys[0x4A] = MIMAS_KEY_J;
    platform->keys[0x4B] = MIMAS_KEY_K;
    platform->keys[0x4C] = MIMAS_KEY_L;
    platform->keys[0x4D] = MIMAS_KEY_M;
    platform->keys[0x4E] = MIMAS_KEY_N;
    platform->keys[0x4F] = MIMAS_KEY_O;
    platform->keys[0x50] = MIMAS_KEY_P;
    platform->keys[0x51] = MIMAS_KEY_Q;
    platform->keys[0x52] = MIMAS_KEY_R;
    platform->keys[0x53] = MIMAS_KEY_S;
    platform->keys[0x54] = MIMAS_KEY_T;
    platform->keys[0x55] = MIMAS_KEY_U;
    platform->keys[0x56] = MIMAS_KEY_V;
    platform->keys[0x57] = MIMAS_KEY_W;
    platform->keys[0x58] = MIMAS_KEY_X;
    platform->keys[0x59] = MIMAS_KEY_Y;
    platform->keys[0x5A] = MIMAS_KEY_Z;

    mimas_bool const register_res = register_window_class();
    if(!register_res) {
        return mimas_false;
    }

    Mimas_Window* const dummy_window = create_native_window((Mimas_Window_Create_Info){.width = 1280, .height = 720, .title = "MIMAS_HELPER_WINDOW", .decorated = mimas_false});
    if(!dummy_window) {
        unregister_window_class();
        free(platform);
        // TODO: Error
        return mimas_false;
    }

    HWND const dummy_hwnd = ((Mimas_Win_Window*)dummy_window->native_window)->handle;
    // If the program is launched with STARTUPINFO, the first call to ShowWindow will ignore the nCmdShow param,
    //   therefore we call it here to clear that behaviour...
    ShowWindow(dummy_hwnd, SW_HIDE);
    // ... and call it again to make sure it's hidden.
    ShowWindow(dummy_hwnd, SW_HIDE);

    MSG msg;
    while (PeekMessageW(&msg, dummy_hwnd, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    if(!mimas_load_wgl(GetDC(dummy_hwnd))) {
        destroy_native_window(dummy_window);
        unregister_window_class();
        free(platform);
        // TODO: Error
        return mimas_false;
    }

    platform->dummy_window = dummy_window;

    Mimas_Internal* const _mimas = _mimas_get_mimas_internal();
    _mimas->platform = platform;

    return mimas_true;
}

void mimas_platform_terminate_with_gl() {
    Mimas_Internal* const _mimas = _mimas_get_mimas_internal();
    Mimas_Win_Platform* const platform = (Mimas_Win_Platform*)_mimas->platform;
    destroy_native_window(platform->dummy_window);
    mimas_unload_wgl();
    unregister_window_class();
    free(platform);
    _mimas->platform = NULL;
}

void mimas_platform_poll_events() {
    MSG msg;
    while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    Mimas_Win_Platform* const platform = (Mimas_Win_Platform*)_mimas_get_mimas_internal()->platform;
    GetKeyboardState(platform->keyboard_state);
}

Mimas_Window* mimas_platform_create_window(Mimas_Window_Create_Info const info) {
    return create_native_window(info);
}

void mimas_platform_destroy_window(Mimas_Window* const window) {
    destroy_native_window(window);
}

void mimas_platform_set_window_pos(Mimas_Window* const window, mimas_i32 const x, mimas_i32 const y) {
    Mimas_Win_Window* const native_window = (Mimas_Win_Window*)window->native_window;
    SetWindowPos(native_window->handle, NULL, x, y, 0, 0, SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_NOACTIVATE);
}

void mimas_platform_get_window_pos(Mimas_Window*const window, mimas_i32* const x, mimas_i32* const y) {
    Mimas_Win_Window* const native_window = (Mimas_Win_Window*)window->native_window;
    RECT window_rect;
    GetWindowRect(native_window->handle, &window_rect);
    *x = window_rect.left;
    *y = window_rect.top;
}

void mimas_platform_set_window_content_pos(Mimas_Window* const window, mimas_i32 const x, mimas_i32 const y) {
    Mimas_Win_Window* const native_window = (Mimas_Win_Window*)window->native_window;
    if(window->decorated) {
        RECT pos = {x, y, x, y};
        // TODO: Make DPI aware.
        AdjustWindowRectEx(&pos, get_window_styles(window), FALSE, get_window_extended_styles(window));
        SetWindowPos(native_window->handle, NULL, pos.left, pos.top, 0, 0, SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_NOACTIVATE);
    } else {
        SetWindowPos(native_window->handle, NULL, x, y, 0, 0, SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_NOACTIVATE);
    }
}

void mimas_platform_get_window_content_pos(Mimas_Window*const window, mimas_i32* const x, mimas_i32* const y) {
    Mimas_Win_Window* const native_window = (Mimas_Win_Window*)window->native_window;
    POINT pos = {0, 0};
    ClientToScreen(native_window->handle, &pos);
    *x = pos.x;
    *y = pos.y;
}

void mimas_platform_set_window_content_size(Mimas_Window*const window, mimas_i32 const width, mimas_i32 const height) {
    Mimas_Win_Window* const native_window = (Mimas_Win_Window*)window->native_window;
    if(window->decorated) {
        RECT rect = {0, 0, width, height};
        // TODO: Make DPI aware.
        AdjustWindowRectEx(&rect, get_window_styles(window), FALSE, get_window_extended_styles(window));
        SetWindowPos(native_window->handle, NULL, 0, 0, rect.right - rect.left, rect.bottom - rect.top, SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_NOACTIVATE);
    } else {
        SetWindowPos(native_window->handle, NULL, 0, 0, width, height, SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_NOACTIVATE);
    }
}

void mimas_platform_get_window_content_size(Mimas_Window*const window, mimas_i32* const width, mimas_i32* const height) {
    Mimas_Win_Window* const native_window = (Mimas_Win_Window*)window->native_window;
    RECT client_rect;
    GetClientRect(native_window->handle, &client_rect);
    *width = client_rect.right;
    *height = client_rect.bottom;
}

void mimas_platform_show_window(Mimas_Window* const window) {
    Mimas_Win_Window* const native_window = (Mimas_Win_Window*)window->native_window;
    ShowWindow(native_window->handle, SW_SHOW);
}

void mimas_platform_hide_window(Mimas_Window* const window) {
    Mimas_Win_Window* const native_window = (Mimas_Win_Window*)window->native_window;
    ShowWindow(native_window->handle, SW_HIDE);
}

void mimas_platform_restore_window(Mimas_Window* const window) {
    Mimas_Win_Window* const native_window = (Mimas_Win_Window*)window->native_window;
    ShowWindow(native_window->handle, SW_RESTORE);
}

void mimas_platform_minimize_window(Mimas_Window* const window) {
    Mimas_Win_Window* const native_window = (Mimas_Win_Window*)window->native_window;
    ShowWindow(native_window->handle, SW_MINIMIZE);
}

void mimas_platform_maximize_window(Mimas_Window* const window) {
    Mimas_Win_Window* const native_window = (Mimas_Win_Window*)window->native_window;
    ShowWindow(native_window->handle, SW_MAXIMIZE);
}

void mimas_platform_set_swap_interval(mimas_i32 const interval) {
    wglSwapIntervalEXT(interval);
}

mimas_i32 mimas_platform_get_swap_interval() {
    return wglGetSwapIntervalEXT();
}

// TODO: Move to input.c
void mimas_platform_set_cursor_mode(Mimas_Window* const window, Mimas_Cursor_Mode const cursor_mode) {
    window->cursor_mode = cursor_mode;
}
