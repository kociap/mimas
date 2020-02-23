#include <win/platform.h>

#include <platform.h>
#include <internal.h>
#include <utils.h>
#include <platform_gl.h>
#include <platform_vk.h>

#include <wingdi.h>

#include <stdlib.h>
#include <string.h>

#include <stdio.h>

#include <hidusage.h>

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
    RECT client;
    GetClientRect(native_window->handle, &client);
    ClientToScreen(native_window->handle, (POINT*)&client.left);
    ClientToScreen(native_window->handle, (POINT*)&client.right);
    ClipCursor(&client);
}

static void release_captured_cursor() {
    ClipCursor(NULL);
}

static void enable_virtual_cursor(Mimas_Window* const window) {
    // Capture in case we lag and the cursor moves out of the window.
    capture_cursor(window);

}

static void disable_virtual_cursor(Mimas_Window* const window) {
    release_captured_cursor();
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
        case client        : return HTCLIENT;
        default            : return HTNOWHERE;
    }
}

static Mimas_Key _translate_scan_code(mimas_i32 const sc) {
    Mimas_Key vk_map[256];
    memset(vk_map, MIMAS_KEY_UNKNOWN, sizeof(vk_map));
    vk_map[VK_ESCAPE] = MIMAS_KEY_ESCAPE;
    vk_map[VK_BACK] = MIMAS_KEY_BACKSPACE;
    vk_map[VK_TAB] = MIMAS_KEY_TAB;
    vk_map[VK_PAUSE] = MIMAS_KEY_PAUSE;
    vk_map[VK_CAPITAL] = MIMAS_KEY_CAPS_LOCK;
    vk_map[VK_SPACE] = MIMAS_KEY_SPACE;
    vk_map[VK_SNAPSHOT] = MIMAS_KEY_PRINT_SCREEN;
    vk_map[0x30] = MIMAS_KEY_0;
    vk_map[0x31] = MIMAS_KEY_1;
    vk_map[0x32] = MIMAS_KEY_2;
    vk_map[0x33] = MIMAS_KEY_3;
    vk_map[0x34] = MIMAS_KEY_4;
    vk_map[0x35] = MIMAS_KEY_5;
    vk_map[0x36] = MIMAS_KEY_6;
    vk_map[0x37] = MIMAS_KEY_7;
    vk_map[0x38] = MIMAS_KEY_8;
    vk_map[0x39] = MIMAS_KEY_9;
    vk_map[0x30] = MIMAS_KEY_0;
    vk_map[0x41] = MIMAS_KEY_A;
    vk_map[0x42] = MIMAS_KEY_B;
    vk_map[0x43] = MIMAS_KEY_C;
    vk_map[0x44] = MIMAS_KEY_D;
    vk_map[0x45] = MIMAS_KEY_E;
    vk_map[0x46] = MIMAS_KEY_F;
    vk_map[0x47] = MIMAS_KEY_G;
    vk_map[0x48] = MIMAS_KEY_H;
    vk_map[0x49] = MIMAS_KEY_I;
    vk_map[0x4A] = MIMAS_KEY_J;
    vk_map[0x4B] = MIMAS_KEY_K;
    vk_map[0x4C] = MIMAS_KEY_L;
    vk_map[0x4D] = MIMAS_KEY_M;
    vk_map[0x4E] = MIMAS_KEY_N;
    vk_map[0x4F] = MIMAS_KEY_O;
    vk_map[0x50] = MIMAS_KEY_P;
    vk_map[0x51] = MIMAS_KEY_Q;
    vk_map[0x52] = MIMAS_KEY_R;
    vk_map[0x53] = MIMAS_KEY_S;
    vk_map[0x54] = MIMAS_KEY_T;
    vk_map[0x55] = MIMAS_KEY_U;
    vk_map[0x56] = MIMAS_KEY_V;
    vk_map[0x57] = MIMAS_KEY_W;
    vk_map[0x58] = MIMAS_KEY_X;
    vk_map[0x59] = MIMAS_KEY_Y;
    vk_map[0x5A] = MIMAS_KEY_Z;
    vk_map[VK_MULTIPLY] = MIMAS_KEY_NUMPAD_MULTIPLY;
    vk_map[VK_ADD] = MIMAS_KEY_NUMPAD_ADD;
    vk_map[VK_SUBTRACT] = MIMAS_KEY_NUMPAD_SUBTRACT;
    vk_map[VK_DECIMAL] = MIMAS_KEY_NUMPAD_DECIMAL;
    vk_map[VK_F1] = MIMAS_KEY_F1;
    vk_map[VK_F2] = MIMAS_KEY_F2;
    vk_map[VK_F3] = MIMAS_KEY_F3;
    vk_map[VK_F4] = MIMAS_KEY_F4;
    vk_map[VK_F5] = MIMAS_KEY_F5;
    vk_map[VK_F6] = MIMAS_KEY_F6;
    vk_map[VK_F7] = MIMAS_KEY_F7;
    vk_map[VK_F8] = MIMAS_KEY_F8;
    vk_map[VK_F9] = MIMAS_KEY_F9;
    vk_map[VK_F10] = MIMAS_KEY_F10;
    vk_map[VK_F11] = MIMAS_KEY_F11;
    vk_map[VK_F12] = MIMAS_KEY_F12;
    vk_map[VK_NUMLOCK] = MIMAS_KEY_NUMLOCK;
    vk_map[VK_SCROLL] = MIMAS_KEY_SCROLL_LOCK;
    UINT const key = MapVirtualKeyW(sc, MAPVK_VSC_TO_VK_EX);
    return vk_map[key];
}

static Mimas_Key translate_scan_code(mimas_i32 const sc, mimas_bool const e0, mimas_bool const e1) {
    // MapVirtualKey doesn't properly translate left- and right- keys, numpad, extended us keyboard keys, 
    // so we do it manually.
    switch(sc) {
        case 0x1c:
            return (MIMAS_KEY_NUMPAD_ENTER & (0 - e0)) | (MIMAS_KEY_ENTER & (0 - !e0));
        case 0x1d:
            return (MIMAS_KEY_RIGHT_CONTROL & (0 - e0)) | (MIMAS_KEY_LEFT_CONTROL & (0 - !e0));
        case 0x38:
            return (MIMAS_KEY_RIGHT_ALT & (0 - e0)) | (MIMAS_KEY_LEFT_ALT & (0 - !e0));
        case 0x2a:
            return MIMAS_KEY_LEFT_SHIFT;
        case 0x36:
            return MIMAS_KEY_RIGHT_SHIFT;
        case 0x52:
            return (MIMAS_KEY_INSERT & (0 - e0)) | (MIMAS_KEY_NUMPAD_0 & (0 - !e0));
        case 0x4f:
            return (MIMAS_KEY_END & (0 - e0)) | (MIMAS_KEY_NUMPAD_1 & (0 - !e0));
        case 0x50:
            return (MIMAS_KEY_DOWN & (0 - e0)) | (MIMAS_KEY_NUMPAD_2 & (0 - !e0));
        case 0x51:
            return (MIMAS_KEY_PAGE_DOWN & (0 - e0)) | (MIMAS_KEY_NUMPAD_3 & (0 - !e0));
        case 0x4b:
            return (MIMAS_KEY_LEFT & (0 - e0)) | (MIMAS_KEY_NUMPAD_4 & (0 - !e0));
        case 0x4c:
            return MIMAS_KEY_NUMPAD_5;
        case 0x4d:
            return (MIMAS_KEY_RIGHT & (0 - e0)) | (MIMAS_KEY_NUMPAD_6 & (0 - !e0));
        case 0x47:
            return (MIMAS_KEY_HOME & (0 - e0)) | (MIMAS_KEY_NUMPAD_7 & (0 - !e0));
        case 0x48:
            return (MIMAS_KEY_UP & (0 - e0)) | (MIMAS_KEY_NUMPAD_8 & (0 - !e0));
        case 0x49:
            return (MIMAS_KEY_PAGE_UP & (0 - e0)) | (MIMAS_KEY_NUMPAD_9 & (0 - !e0));
        case 0x53:
            return (MIMAS_KEY_DELETE & (0 - e0)) | (MIMAS_KEY_NUMPAD_DECIMAL & (0 - !e0));
        case 0x35:
            return (MIMAS_KEY_NUMPAD_DIVIDE & (0 - e0)) | (MIMAS_KEY_SLASH & (0 - !e0));
        case 0x27:
            return MIMAS_KEY_SEMICOLON;
        case 0x28:
            return MIMAS_KEY_APOSTROPHE;
        case 0x29:
            return MIMAS_KEY_TICK;
        case 0xc:
            return MIMAS_KEY_MINUS;
        case 0xd:
            return MIMAS_KEY_EQUALS;
        case 0x33:
            return MIMAS_KEY_COMMA;
        case 0x34:
            return MIMAS_KEY_DOT;
        case 0x1a:
            return MIMAS_KEY_LEFT_BRACKET;
        case 0x1b:
            return MIMAS_KEY_RIGHT_BRACKET;
        case 0x2b:
            return MIMAS_KEY_BACKWARD_SLASH;

        default:
            // Translate remaining keys.
            return _translate_scan_code(sc);
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
        } break;

        case WM_SIZE: {
            // TODO: Handle minimmize separately? (This is specified in the wparam parameter)
            if (window && window->callbacks.window_resize) {
                window->callbacks.window_resize(window, GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam), 
                    window->callbacks.window_resize_data);
            }
        } break;

        case WM_NCCALCSIZE: {
            if(wparam == TRUE && !window->decorated) {
                return 0;
            }
        } break;

        case WM_NCHITTEST: {
            RECT window_rect;
            GetWindowRect(hwnd, &window_rect);
            RECT client_rect;
            GetClientRect(hwnd, &client_rect);
            ClientToScreen(hwnd, (POINT*)&client_rect.left);
            ClientToScreen(hwnd, (POINT*)&client_rect.right);
            if(window->callbacks.hittest) {
                Mimas_Rect const _window_rect = {window_rect.left, window_rect.top, window_rect.bottom, window_rect.right};
                Mimas_Rect const _client_rect = {client_rect.left, client_rect.top, client_rect.bottom, client_rect.right};
                Mimas_Hittest_Result const r = window->callbacks.hittest(window, GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam), _window_rect, _client_rect);
                mimas_i32 const hit[] = {
                    [MIMAS_HITTEST_TOP] = HTTOP,
                    [MIMAS_HITTEST_BOTTOM] = HTBOTTOM,
                    [MIMAS_HITTEST_LEFT] = HTLEFT,
                    [MIMAS_HITTEST_RIGHT] = HTRIGHT,
                    [MIMAS_HITTEST_TOP_LEFT] = HTTOPLEFT,
                    [MIMAS_HITTEST_TOP_RIGHT] = HTTOPRIGHT,
                    [MIMAS_HITTEST_BOTTOM_LEFT] = HTBOTTOMLEFT,
                    [MIMAS_HITTEST_BOTTOM_RIGHT] = HTBOTTOMRIGHT,
                    [MIMAS_HITTEST_CLIENT] = HTCLIENT,
                    [MIMAS_HITTEST_TITLEBAR] = HTCAPTION,
                    [MIMAS_HITTEST_MINIMIZE] = HTMINBUTTON,
                    [MIMAS_HITTEST_MAXIMIZE] = HTMAXBUTTON,
                    [MIMAS_HITTEST_CLOSE] = HTCLOSE,
                    [MIMAS_HITTEST_NOWHERE] = HTNOWHERE,
                };
                return hit[r];
            } else if(!window->decorated) {
                return window_hit_test(GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam), window_rect);
            }
        } break;

        case WM_SETFOCUS: {
            if(window->cursor_mode == MIMAS_CURSOR_CAPTURED) {
                capture_cursor(window);
            } else if(window->cursor_mode == MIMAS_CURSOR_VIRTUAL) {
                enable_virtual_cursor(window);
            }

            Mimas_Internal* const _mimas = _mimas_get_mimas_internal();
            _mimas->active_window = window;

            if(window->callbacks.window_activate) {
                window->callbacks.window_activate(window, mimas_true, window->callbacks.window_activate_data);
            }
        } break;

        case WM_KILLFOCUS: {
            if(window->cursor_mode == MIMAS_CURSOR_CAPTURED) {
                release_captured_cursor();
            } else if(window->cursor_mode == MIMAS_CURSOR_VIRTUAL) {
                disable_virtual_cursor(window);
            }

            Mimas_Internal* const _mimas = _mimas_get_mimas_internal();
            _mimas->active_window = NULL;

            if(window->callbacks.key) {
                for(mimas_u32 i = 0; i < ARRAY_SIZE(window->keys); ++i) {
                    if(window->keys[i] != MIMAS_KEY_RELEASE) {
                        window->keys[i] = MIMAS_KEY_RELEASE;
                        window->callbacks.key(window, i, MIMAS_KEY_RELEASE, window->callbacks.key_data);
                    }
                }
            }

            if(window->callbacks.window_activate) {
                window->callbacks.window_activate(window, mimas_false, window->callbacks.window_activate_data);
            }
        } break;

        case WM_INPUT: {
            Mimas_Win_Platform* const platform = (Mimas_Win_Platform*)_mimas_get_mimas_internal()->platform;
            UINT size;
            GetRawInputData((HRAWINPUT)lparam,  RID_INPUT, NULL, &size, sizeof(RAWINPUTHEADER));
            RAWINPUT* const ri = malloc(size);
            GetRawInputData((HRAWINPUT)lparam, RID_INPUT, ri, &size, sizeof(RAWINPUTHEADER));
            switch(ri->header.dwType) {
                case RIM_TYPEKEYBOARD: {
                    RAWKEYBOARD* const kb = &ri->data.keyboard;
                    mimas_bool const e0 = !!(kb->Flags & RI_KEY_E0);
                    mimas_bool const e1 = !!(kb->Flags & RI_KEY_E1);
                    Mimas_Key_Action const action = !(kb->Flags & RI_KEY_BREAK);
                    Mimas_Key const key = translate_scan_code(kb->MakeCode, e0, e1);
                    if(key != MIMAS_KEY_UNKNOWN) {
                        platform->key_state[key] = action;
                    }
                } break;

                case RIM_TYPEMOUSE: {
                    RAWMOUSE* const mouse = &ri->data.mouse;
                    if(mouse->usButtonFlags & RI_MOUSE_LEFT_BUTTON_DOWN) {
                        platform->key_state[MIMAS_MOUSE_LEFT_BUTTON] = MIMAS_KEY_PRESS;
                    } else if(mouse->usButtonFlags & RI_MOUSE_LEFT_BUTTON_UP) {
                        platform->key_state[MIMAS_MOUSE_LEFT_BUTTON] = MIMAS_KEY_RELEASE;
                    }

                    if(mouse->usButtonFlags & RI_MOUSE_RIGHT_BUTTON_DOWN) {
                        platform->key_state[MIMAS_MOUSE_RIGHT_BUTTON] = MIMAS_KEY_PRESS;
                    } else if(mouse->usButtonFlags & RI_MOUSE_RIGHT_BUTTON_UP) {
                        platform->key_state[MIMAS_MOUSE_RIGHT_BUTTON] = MIMAS_KEY_RELEASE;
                    }

                    if(mouse->usButtonFlags & RI_MOUSE_MIDDLE_BUTTON_DOWN) {
                        platform->key_state[MIMAS_MOUSE_MIDDLE_BUTTON] = MIMAS_KEY_PRESS;
                    } else if(mouse->usButtonFlags & RI_MOUSE_MIDDLE_BUTTON_UP) {
                        platform->key_state[MIMAS_MOUSE_MIDDLE_BUTTON] = MIMAS_KEY_RELEASE;
                    }

                    if(mouse->usButtonFlags & RI_MOUSE_BUTTON_4_DOWN) {
                        platform->key_state[MIMAS_MOUSE_THUMB_BUTTON_1] = MIMAS_KEY_PRESS;
                    } else if(mouse->usButtonFlags & RI_MOUSE_BUTTON_4_UP) {
                        platform->key_state[MIMAS_MOUSE_THUMB_BUTTON_1] = MIMAS_KEY_RELEASE;
                    }

                    if(mouse->usButtonFlags & RI_MOUSE_BUTTON_5_DOWN) {
                        platform->key_state[MIMAS_MOUSE_THUMB_BUTTON_2] = MIMAS_KEY_PRESS;
                    } else if(mouse->usButtonFlags & RI_MOUSE_BUTTON_5_UP) {
                        platform->key_state[MIMAS_MOUSE_THUMB_BUTTON_2] = MIMAS_KEY_RELEASE;
                    }
                } break;

                case RIM_TYPEHID: {

                } break;
            }
        } break;

        case WM_KEYUP:
        case WM_KEYDOWN: {
            mimas_u32 const sc = (lparam & 0xFF0000) >> 16;
            mimas_bool const extended = !!(lparam & 0x800000);
            mimas_bool const key_was_up = !!(lparam & 0x80000000);
            Mimas_Key_Action action;
            if(msg == WM_KEYDOWN) {
                action = key_was_up ? MIMAS_KEY_PRESS : MIMAS_KEY_REPEAT;
            } else {
                action = MIMAS_KEY_RELEASE;
            }

            Mimas_Key const key = translate_scan_code(sc, extended, 0);
            if(key == MIMAS_KEY_UNKNOWN) {
                break;
            }

            window->keys[key] = action;

            if(window->callbacks.key) {
                window->callbacks.key(window, key, action, window->callbacks.key_data);
            }
        } break;

        case WM_LBUTTONDOWN:
        case WM_MBUTTONDOWN:
        case WM_RBUTTONDOWN:
        case WM_XBUTTONDOWN: 
        case WM_LBUTTONUP:
        case WM_MBUTTONUP:
        case WM_RBUTTONUP:
        case WM_XBUTTONUP: {
            if(window->callbacks.mouse_button) {
                Mimas_Mouse_Button_Action const action = (msg == WM_LBUTTONUP || msg == WM_MBUTTONUP || msg == WM_RBUTTONUP || msg == WM_XBUTTONUP);
                mimas_bool const is_lmb = (msg == WM_LBUTTONUP || msg == WM_LBUTTONDOWN);
                mimas_bool const is_rmb = (msg == WM_RBUTTONUP || msg == WM_RBUTTONDOWN);
                mimas_bool const is_mmb = (msg == WM_MBUTTONUP || msg == WM_MBUTTONDOWN);
                mimas_bool const is_xmb = (msg == WM_XBUTTONUP || msg == WM_XBUTTONDOWN);
                Mimas_Key const button = MIMAS_MOUSE_LEFT_BUTTON * is_lmb | MIMAS_MOUSE_RIGHT_BUTTON * is_rmb | MIMAS_MOUSE_MIDDLE_BUTTON * is_mmb;
                // TODO: Temporarily because we don't have enough buttons.
                if(is_lmb || is_rmb || is_mmb) {
                    window->callbacks.mouse_button(window, button, action, window->callbacks.mouse_button_data);
                }
            }

            return 0;
        } break;

        case WM_MOUSEMOVE: {
            mimas_i32 const x = GET_X_LPARAM(lparam);
            mimas_i32 const y = GET_Y_LPARAM(lparam);

            if(window->callbacks.cursor_pos) {
                window->callbacks.cursor_pos(window, x, y, window->callbacks.cursor_pos_data);
            }

            return 0;
        } break;

        case WM_CLOSE: {
            window->close_requested = mimas_true;
            return 0;
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
        .hInstance = NULL,
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
    HWND const hwnd = CreateWindowEx(WS_EX_APPWINDOW, MIMAS_WINDOW_CLASS_NAME, wtitle, style, CW_USEDEFAULT, CW_USEDEFAULT, info.width, info.height, NULL, NULL, NULL, NULL);
    free(wtitle);

    if(!hwnd) {
        free(window);
        // TODO: Error
        return NULL;
    }
    
    Mimas_Internal* const _mimas = _mimas_get_mimas_internal();
    HDC const hdc = GetDC(hwnd);
    if(_mimas->backend == MIMAS_BACKEND_GL) {
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
    }

    Mimas_Win_Window* native_window = (Mimas_Win_Window*)malloc(sizeof(Mimas_Win_Window));
    native_window->handle = hwnd;
    native_window->hdc = hdc;
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

mimas_bool mimas_platform_init() {
    Mimas_Win_Platform* const platform = (Mimas_Win_Platform*)malloc(sizeof(Mimas_Win_Platform));
    memset(platform, 0, sizeof(Mimas_Win_Platform));

    mimas_bool const register_res = register_window_class();
    if(!register_res) {
        return mimas_false;
    }

    Mimas_Internal* const _mimas = _mimas_get_mimas_internal();
    _mimas->platform = platform;
    Mimas_Window* const dummy_window = create_native_window((Mimas_Window_Create_Info){.width = 1280, .height = 720, .title = "MIMAS_HELPER_WINDOW", .decorated = mimas_false});
    if(!dummy_window) {
        unregister_window_class();
        free(platform);
        _mimas->platform = NULL;
        // TODO: Error
        return mimas_false;
    }
    platform->dummy_window = dummy_window;

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

    if(_mimas->backend == MIMAS_BACKEND_GL) {
        if(!mimas_platform_init_gl_backend()) {
            destroy_native_window(dummy_window);
            unregister_window_class();
            free(platform);
            _mimas->platform = NULL;
            return mimas_false;
        }
    } else {
        if(!mimas_platform_init_vk_backend()) {
            unregister_window_class();
            free(platform);
            _mimas->platform = NULL;
            return mimas_false;
        }
    }

    // Install raw input listener 
    RAWINPUTDEVICE rid[] = {
        {.usUsagePage = HID_USAGE_PAGE_GENERIC, .usUsage = HID_USAGE_GENERIC_MOUSE, .dwFlags = RIDEV_INPUTSINK, .hwndTarget = dummy_hwnd},
        {.usUsagePage = HID_USAGE_PAGE_GENERIC, .usUsage = HID_USAGE_GENERIC_KEYBOARD, .dwFlags = RIDEV_INPUTSINK, .hwndTarget = dummy_hwnd},
        {.usUsagePage = HID_USAGE_PAGE_GENERIC, .usUsage = HID_USAGE_GENERIC_JOYSTICK, .dwFlags = RIDEV_INPUTSINK, .hwndTarget = dummy_hwnd},
        {.usUsagePage = HID_USAGE_PAGE_GENERIC, .usUsage = HID_USAGE_GENERIC_GAMEPAD, .dwFlags = RIDEV_INPUTSINK, .hwndTarget = dummy_hwnd}
    };
    RegisterRawInputDevices(rid, 4, sizeof(RAWINPUTDEVICE));

    return mimas_true;
}

void mimas_platform_terminate(Mimas_Backend const backend) {
    if(backend == MIMAS_BACKEND_GL) {
        mimas_platform_terminate_gl_backend();
    } else {
        mimas_platform_terminate_vk_backend();
    }

    Mimas_Internal* const _mimas = _mimas_get_mimas_internal();
    Mimas_Win_Platform* const platform = (Mimas_Win_Platform*)_mimas->platform;
    destroy_native_window(platform->dummy_window);
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

// TODO: Move to input.c
void mimas_platform_set_cursor_mode(Mimas_Window* const window, Mimas_Cursor_Mode const cursor_mode) {
    window->cursor_mode = cursor_mode;
}
