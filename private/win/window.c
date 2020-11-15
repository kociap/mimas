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

static mimas_u32 get_default_window_styles(void) {
    return WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP | WS_THICKFRAME | WS_CAPTION | WS_SYSMENU | WS_MAXIMIZEBOX | WS_MINIMIZEBOX;
}

static mimas_u32 get_fullscreen_borderless_window_styles(void) {
    return WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP;
}

static mimas_u32 get_default_extended_window_styles(void) {
    return WS_EX_APPWINDOW;
}

static Mimas_Rect get_client_rect(Mimas_Window* const window) {
    Mimas_Win_Window* const native_window = (Mimas_Win_Window*)window->native_window;
    RECT client;
    GetClientRect(native_window->handle, &client);
    return *(Mimas_Rect*)&client;
}

static void clip_cursor_to_window(Mimas_Window* const window, Mimas_Rect const* const region) {
    Mimas_Win_Window* const native_window = (Mimas_Win_Window*)window->native_window;
    RECT clip_region = {.left = region->left, .top = region->top, .right = region->right, .bottom = region->bottom};
    ClientToScreen(native_window->handle, (POINT*)&clip_region.left);
    ClientToScreen(native_window->handle, (POINT*)&clip_region.right);
    ClipCursor(&clip_region);
}

static void lock_cursor_to_window(Mimas_Window* const window) {
    Mimas_Win_Window* const native_window = (Mimas_Win_Window*)window->native_window;
    RECT client;
    GetClientRect(native_window->handle, &client);
    RECT clip_region = {.left = client.right / 2, .right = client.right / 2 + 1, .top = client.bottom / 2, .bottom = client.bottom / 2 + 1};
    ClientToScreen(native_window->handle, (POINT*)&clip_region.left);
    ClientToScreen(native_window->handle, (POINT*)&clip_region.right);
    ClipCursor(&clip_region);
}

static void fit_window_to_display(Mimas_Window* const window, Mimas_Display* const display) {
    Mimas_Win_Display* const native_display = (Mimas_Win_Display*)display->native_display;
    MONITORINFO info = { .cbSize = sizeof(MONITORINFO) };
    GetMonitorInfoW(native_display->hmonitor, &info);
    Mimas_Win_Window* const native_window = (Mimas_Win_Window*)window->native_window;
    SetWindowPos(native_window->handle, NULL, info.rcMonitor.left, info.rcMonitor.top, 
                 info.rcMonitor.right - info.rcMonitor.left, 
                 info.rcMonitor.bottom - info.rcMonitor.top, 
                 SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOCOPYBITS);
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
    UNUSED(e1);
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
    Mimas_Internal* const _mimas = _mimas_get_mimas_internal();
    Mimas_Window* const window = GetPropW(hwnd, L"Mimas_Window");
    mimas_bool const global_input_handler = (mimas_bool)GetPropW(hwnd, L"global_input_handler");
    if(global_input_handler) {
        if(msg == WM_INPUT) {
            Mimas_Win_Platform* const platform = (Mimas_Win_Platform*)_mimas->platform;
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
                        Mimas_Window* const active_window = _mimas->active_window;
                        if(active_window && active_window->callbacks.key) {
                            active_window->keys[key] = action;
                            active_window->callbacks.key(active_window, key, action, active_window->callbacks.key_data);
                        }
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

                    if(mouse->usButtonFlags & RI_MOUSE_WHEEL) {
                        if(_mimas->active_window && _mimas->active_window->callbacks.scroll) {
                            float const delta = (short)mouse->usButtonData / WHEEL_DELTA;
                            _mimas->active_window->callbacks.scroll(_mimas->active_window, 0, delta, _mimas->active_window->callbacks.scroll_data);
                        }
                    }

                    if(mouse->usButtonFlags & RI_MOUSE_HWHEEL) {
                        if(_mimas->active_window && _mimas->active_window->callbacks.scroll) {
                            float const delta = (short)mouse->usButtonData / WHEEL_DELTA;
                            _mimas->active_window->callbacks.scroll(_mimas->active_window, delta, 0, _mimas->active_window->callbacks.scroll_data);
                        }
                    }

                    // mimas_bool const mouse_move_absolute = !!(mouse->usFlags & MOUSE_MOVE_ABSOLUTE);
                    mimas_bool const mouse_move_relative = !(mouse->usFlags & MOUSE_MOVE_ABSOLUTE);
                    if(mouse_move_relative && _mimas->locked_cursor_window) {
                        Mimas_Window* window = _mimas->locked_cursor_window;
                        _mimas->cursor_virtual_pos_x += mouse->lLastX;
                        _mimas->cursor_virtual_pos_y += mouse->lLastY;
                        if(window->callbacks.cursor_pos) {
                            window->callbacks.cursor_pos(window, _mimas->cursor_virtual_pos_x, _mimas->cursor_virtual_pos_y, window->callbacks.cursor_pos_data);
                        }
                    }
                    // TODO: absolute mouse move
                    // TODO: virtual desktop
                } break;

                case RIM_TYPEHID: {

                } break;
            }
        }

        return DefWindowProcW(hwnd, msg, wparam, lparam);
    } else {
        {
            // If a window receives mouse button events, it's active.
            mimas_bool const mouse_button_down = msg == WM_LBUTTONDOWN || msg == WM_RBUTTONDOWN || msg == WM_MBUTTONDOWN || msg == WM_XBUTTONDOWN;
            // We clip/lock the cursor on each mouse button event. The reason is that we might not clip/lock it when a window is being activated due to 
            // non-client activate (we want to avoid cursor clipping when the user is e.g. clicking the titlebar, dragging the window).
            if(window && _mimas->locked_cursor_window == window && mouse_button_down) {
                lock_cursor_to_window(window);
            } else if(window && _mimas->clipped_cursor_window == window && mouse_button_down) {
                Mimas_Rect const client = get_client_rect(window);
                Mimas_Rect const clip_rect = crop_rect(&_mimas->cursor_clip_region, &client);
                clip_cursor_to_window(window, &clip_rect);
            }
        }

        switch(msg) {
            case WM_MOUSEACTIVATE: {
                Mimas_Win_Window* const native_window = (Mimas_Win_Window*)window->native_window;
                LRESULT const hit = LOWORD(lparam);
                native_window->non_client_activate = (hit != HTCLIENT);
            } break;

            case WM_CAPTURECHANGED: {
                // TODO: Figure out what this does
                if(lparam == 0) {
                    Mimas_Win_Window* const native_window = (Mimas_Win_Window*)window->native_window;
                    native_window->non_client_activate = mimas_false;
                }
            } break;

            case WM_ACTIVATE: {
                if(!window->decorated) {
                    MARGINS const margins = {1, 1, 1, 1};
                    DwmExtendFrameIntoClientArea(hwnd, &margins);
                    SetWindowPos(hwnd, NULL, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER);
                }

                WORD const active = LOWORD(wparam);
                if(active) {
                    _mimas->active_window = window;

                    Mimas_Win_Window* const native_window = (Mimas_Win_Window*)window->native_window;
                    // Do not clip the cursor if the window is activated by mouse click in non-client area
                    if(!native_window->non_client_activate) {
                        if(_mimas->locked_cursor_window == window) {
                            lock_cursor_to_window(window);
                        } else if(_mimas->clipped_cursor_window == window) {
                            Mimas_Rect const client = get_client_rect(window);
                            Mimas_Rect const clip_rect = crop_rect(&_mimas->cursor_clip_region, &client);
                            clip_cursor_to_window(window, &clip_rect);
                        }
                    }

                    if(window->callbacks.window_activate) {
                        window->callbacks.window_activate(window, mimas_true, window->callbacks.window_activate_data);
                    }
                } else {
                    for(mimas_u32 i = 0; i < ARRAY_SIZE(window->keys); ++i) {
                        if(window->keys[i] != MIMAS_KEY_RELEASE && window->callbacks.key) {
                            window->callbacks.key(window, i, MIMAS_KEY_RELEASE, window->callbacks.key_data);
                        }
                        window->keys[i] = MIMAS_KEY_RELEASE;
                    }

                    _mimas->active_window = NULL;

                    if(window->callbacks.window_activate) {
                        window->callbacks.window_activate(window, mimas_false, window->callbacks.window_activate_data);
                    }

                    if(_mimas->locked_cursor_window == window) {
                        ClipCursor(NULL);
                    } else if(_mimas->clipped_cursor_window == window) {
                        ClipCursor(NULL);
                    }

                    // If input sink is disabled and the application is losing focus (lparam is 0, which means 
                    // none of our windows are being activated), we also want to clear all the global keys to make sure that 
                    // none of the keys will be reported as pressed when the release happens after focus loss.
                    Mimas_Win_Platform* const platform = (Mimas_Win_Platform*)_mimas->platform;
                    if(lparam == 0 && platform->disable_raw_input_sink) {
                        for(mimas_u32 i = 0; i < ARRAY_SIZE(platform->key_state); ++i) {
                            platform->key_state[i] = MIMAS_KEY_RELEASE;
                        }
                    }
                }
            } break;

            case WM_NCCALCSIZE: {
                if(wparam == TRUE && !window->decorated) {
                    return 0;
                }
            } break;

            case WM_NCHITTEST: {
                if(window->display) {
                    // Do not run user-defined hittest on fullscreen windows
                    break;
                }

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
                    LRESULT const hit[] = {
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

            case WM_SETCURSOR: {
                if(LOWORD(lparam) == HTCLIENT) {
                    SetCursor((HCURSOR)window->cursor);
                    return TRUE;
                }

            } break;

            case WM_SIZE: {
                // TODO: Handle minimmize separately? (This is specified in the wparam parameter)
                if (window && window->callbacks.window_resize) {
                    window->callbacks.window_resize(window, GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam), 
                        window->callbacks.window_resize_data);
                }
            } break;

            case WM_LBUTTONDOWN:
            case WM_LBUTTONUP: {
                if(window->callbacks.mouse_button) {
                    window->callbacks.mouse_button(window, MIMAS_MOUSE_LEFT_BUTTON, msg == WM_LBUTTONDOWN, window->callbacks.mouse_button_data);
                }
                return TRUE;
            } break;

            case WM_RBUTTONDOWN:
            case WM_RBUTTONUP: {
                if(window->callbacks.mouse_button) {
                    window->callbacks.mouse_button(window, MIMAS_MOUSE_RIGHT_BUTTON, msg == WM_RBUTTONDOWN, window->callbacks.mouse_button_data);
                }
                return TRUE;
            } break;

            case WM_MBUTTONDOWN:
            case WM_MBUTTONUP: {
                if(window->callbacks.mouse_button) {
                    window->callbacks.mouse_button(window, MIMAS_MOUSE_MIDDLE_BUTTON, msg == WM_MBUTTONDOWN, window->callbacks.mouse_button_data);
                }
                return TRUE;
            } break;

            case WM_XBUTTONDOWN: 
            case WM_XBUTTONUP: {
                if(window->callbacks.mouse_button) {
                    WORD const hw = HIWORD(wparam);
                    Mimas_Key const key = (MIMAS_MOUSE_THUMB_BUTTON_1 & (0 - !!(hw & XBUTTON1))) | (MIMAS_MOUSE_THUMB_BUTTON_2 & (0 - !!(hw & XBUTTON2)));
                    window->callbacks.mouse_button(window, key, msg == WM_XBUTTONDOWN, window->callbacks.mouse_button_data);
                }
                return TRUE;
            } break;

            case WM_MOUSEMOVE: {
                if(!_mimas->locked_cursor_window && window->callbacks.cursor_pos) {
                    window->callbacks.cursor_pos(window, GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam), window->callbacks.cursor_pos_data);
                }

                return 0;
            } break;

            case WM_CHAR: {
                Mimas_Win_Window* native_window = (Mimas_Win_Window*)window->native_window;
                mimas_char32 const surrogate = wparam;
                if(native_window->cooking_character) {
                    native_window->cooking_character = mimas_false;
                    mimas_char32 const high_surrogate = native_window->cooked_character;
                    // Subtract 0xD800 from high surrogate. Subtract 0xDC00 from low surrogate.
                    // Subtract 0x10000 from the merged surrogates.
                    // 0x35FDC00 = (0xD800 << 10) + 0xDC00 - 0x10000
                    mimas_char32 const cooked_char = (((high_surrogate & 0x3FF) << 10) | (surrogate & 0x3FF)) - 0x35FDC00;
                    native_window->cooked_character = cooked_char;
                } else {
                    // U+0000 to U+D7FF and U+E000 to U+FFFF are encoded as single 16bit chars.
                    if(surrogate <= 0xD7FF || surrogate >= 0xE000) {
                        native_window->cooked_character = surrogate;
                    } else {
                        native_window->cooking_character = mimas_true;
                        native_window->cooked_character = surrogate;
                        return 0;
                    }
                }

                if(window->callbacks.character) {
                    mimas_char32 const cooked_char = native_window->cooked_character;
                    native_window->cooking_character = mimas_false;
                    window->callbacks.character(window, cooked_char, window->callbacks.character_data);
                }

                return 0;
            } break;

            case WM_CLOSE: {
                window->close_requested = mimas_true;
                return 0;
            } break;
        }

        return DefWindowProcW(hwnd, msg, wparam, lparam);
    }
}

// Returns 1 if succeeded, 0 otherwise
static mimas_bool register_window_class(void) {
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
static mimas_bool unregister_window_class(void) {
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
    memset(native_window, 0, sizeof(Mimas_Win_Window));
    native_window->handle = hwnd;
    native_window->hdc = hdc;
    window->native_window = native_window;

    SetProp(hwnd, L"Mimas_Window", (HANDLE)window);

    if(!info.decorated) {
        MARGINS const margins = {1, 1, 1, 1};
        DwmExtendFrameIntoClientArea(hwnd, &margins);
        SetWindowPos(hwnd, NULL, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER);
    }

    window->cursor = _mimas->default_cursor;

    return window;
}

static void destroy_native_window(Mimas_Window* const window) {
    Mimas_Win_Window* const native_window = (Mimas_Win_Window*)window->native_window;
    ReleaseDC(native_window->handle, native_window->hdc);
    DestroyWindow(native_window->handle);
    free(native_window);
    free(window);
}

mimas_bool mimas_platform_init(Mimas_Backend const backend, Mimas_Init_Options const* const options) {
    Mimas_Internal* const _mimas = _mimas_get_mimas_internal();
    // TODO: We do not free _mimas->displays anywhere as far as I know, so it's an obvious leak
    _mimas->displays = _mimas_get_connected_displays(&_mimas->display_count);
    Mimas_Win_Platform* const platform = (Mimas_Win_Platform*)malloc(sizeof(Mimas_Win_Platform));
    if(!platform) {
        // TODO: Error
        return mimas_false;
    }
    memset(platform, 0, sizeof(Mimas_Win_Platform));
    _mimas->platform = platform;

    platform->disable_raw_input_sink = options->capture_input_when_application_is_out_of_focus;

    mimas_bool const register_res = register_window_class();
    if(!register_res) {
        // TODO: Error
        free(platform);
        _mimas->platform = NULL;
        return mimas_false;
    }

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
    SetPropW(dummy_hwnd, L"global_input_handler", (HANDLE)mimas_true);
    // If the program is launched with STARTUPINFO, the first call to ShowWindow will ignore the nCmdShow param,
    // therefore we call it here to clear that behaviour...
    ShowWindow(dummy_hwnd, SW_HIDE);
    // ...and call it again to make sure it's hidden.
    ShowWindow(dummy_hwnd, SW_HIDE);

    MSG msg;
    while (PeekMessageW(&msg, dummy_hwnd, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    if(backend == MIMAS_BACKEND_GL) {
        if(!mimas_platform_init_gl_backend()) {
            destroy_native_window(dummy_window);
            unregister_window_class();
            free(platform);
            _mimas->platform = NULL;
            return mimas_false;
        }
    } else {
        if(!mimas_platform_init_vk_backend()) {
            destroy_native_window(dummy_window);
            unregister_window_class();
            free(platform);
            _mimas->platform = NULL;
            return mimas_false;
        }
    }

    _mimas->default_cursor = mimas_platform_create_standard_cursor(MIMAS_CURSOR_ARROW);
    _mimas_install_input_listener();

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
    HWND const dummy_hwnd = ((Mimas_Win_Window*)platform->dummy_window->native_window)->handle;
    _mimas_uninstall_input_listener();
    mimas_platform_destroy_cursor(_mimas->default_cursor);
    destroy_native_window(platform->dummy_window);
    unregister_window_class();
    free(platform);
    _mimas->platform = NULL;
    _mimas_free_displays(_mimas->displays, _mimas->display_count);
}

void mimas_platform_poll_events(void) {
    Mimas_Internal* const _mimas = _mimas_get_mimas_internal();
    MSG msg;
    while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        if(msg.message == WM_QUIT) {
            Mimas_Window_List_Element* list_elem = _mimas->window_list;
            while(list_elem) {
                Mimas_Window* const window = list_elem->window;
                window->close_requested = mimas_true;
                if(window->callbacks.close_requested) {
                    window->callbacks.close_requested(window, window->callbacks.close_requested_data);
                }
                list_elem = list_elem->next;
            }
        } else {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

    }
}

void mimas_platform_fullscreen_window(Mimas_Window* const window, Mimas_Display* const display) {
    // TODO: cursor locking/clipping when fullscreening
    if(display) {
        window->display = display;
        Mimas_Win_Window* const native_window = (Mimas_Win_Window*)window->native_window;
        SetWindowLongPtrW(native_window->handle, GWL_STYLE, get_fullscreen_borderless_window_styles());
        fit_window_to_display(window, display);
        SetWindowPos(native_window->handle, HWND_TOP, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE);
        // For some reason changing window styles hides the window,
        // so we force it to appear.
        ShowWindow(native_window->handle, SW_SHOWNOACTIVATE);
    } else if(window->display) {
        window->display = NULL;
        Mimas_Win_Window* const native_window = (Mimas_Win_Window*)window->native_window;
        SetWindowLongPtrW(native_window->handle, GWL_STYLE, get_default_window_styles());
        SetWindowPos(native_window->handle, NULL, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE);
        // For some reason changing window styles hides the window,
        // so we force it to appear.
        ShowWindow(native_window->handle, SW_SHOWNOACTIVATE);
    }
}

Mimas_Window* mimas_platform_create_window(Mimas_Window_Create_Info const info) {
    Mimas_Window* const window = create_native_window(info);
    return window;
}

void mimas_platform_destroy_window(Mimas_Window* const window) {
    destroy_native_window(window);
}

void mimas_platform_set_window_title(Mimas_Window* const window, char const* title) {
    Mimas_Win_Window* const native_window = (Mimas_Win_Window*)window->native_window;
    int const wtitle_buffer_size = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, title, -1, NULL, 0);
    wchar_t* const wtitle = malloc(sizeof(wchar_t) * wtitle_buffer_size);
    MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, title, -1, wtitle, wtitle_buffer_size);
    SetWindowTextW(native_window->handle, wtitle);
    free(wtitle);
}

void mimas_platform_set_window_pos(Mimas_Window* const window, mimas_i32 const x, mimas_i32 const y) {
    if(!window->display) {
        Mimas_Win_Window* const native_window = (Mimas_Win_Window*)window->native_window;
        SetWindowPos(native_window->handle, NULL, x, y, 0, 0, SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_NOACTIVATE);

        Mimas_Internal* const _mimas = _mimas_get_mimas_internal();
        if(_mimas->active_window == window) {
            if(_mimas->locked_cursor_window == window) {
                lock_cursor_to_window(window);
            } else if(_mimas->clipped_cursor_window == window) {
                Mimas_Rect const client = get_client_rect(window);
                Mimas_Rect const clip_rect = crop_rect(&_mimas->cursor_clip_region, &client);
                clip_cursor_to_window(window, &clip_rect);
            }
        }
    }
}

void mimas_platform_get_window_pos(Mimas_Window*const window, mimas_i32* const x, mimas_i32* const y) {
    Mimas_Win_Window* const native_window = (Mimas_Win_Window*)window->native_window;
    RECT window_rect;
    GetWindowRect(native_window->handle, &window_rect);
    *x = window_rect.left;
    *y = window_rect.top;
}

void mimas_platform_set_window_content_pos(Mimas_Window* const window, mimas_i32 const x, mimas_i32 const y) {
    if(!window->display) {
        Mimas_Win_Window* const native_window = (Mimas_Win_Window*)window->native_window;
        if(window->decorated) {
            RECT pos = {x, y, x, y};
            // TODO: Make DPI aware.
            AdjustWindowRectEx(&pos, get_default_window_styles(), FALSE, get_default_extended_window_styles());
            SetWindowPos(native_window->handle, NULL, pos.left, pos.top, 0, 0, SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_NOACTIVATE);
        } else {
            SetWindowPos(native_window->handle, NULL, x, y, 0, 0, SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_NOACTIVATE);
        }

        Mimas_Internal* const _mimas = _mimas_get_mimas_internal();
        if(_mimas->active_window == window) {
            if(_mimas->locked_cursor_window == window) {
                lock_cursor_to_window(window);
            } else if(_mimas->clipped_cursor_window == window) {
                Mimas_Rect const client = get_client_rect(window);
                Mimas_Rect const clip_rect = crop_rect(&_mimas->cursor_clip_region, &client);
                clip_cursor_to_window(window, &clip_rect);
            }
        }
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
    if(!window->display) {
        Mimas_Win_Window* const native_window = (Mimas_Win_Window*)window->native_window;
        if(window->decorated) {
            RECT rect = {0, 0, width, height};
            // TODO: Make DPI aware.
            AdjustWindowRectEx(&rect, get_default_window_styles(), FALSE, get_default_extended_window_styles());
            SetWindowPos(native_window->handle, NULL, 0, 0, rect.right - rect.left, rect.bottom - rect.top, SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_NOACTIVATE);
        } else {
            SetWindowPos(native_window->handle, NULL, 0, 0, width, height, SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_NOACTIVATE);
        }

        Mimas_Internal* const _mimas = _mimas_get_mimas_internal();
        if(_mimas->active_window == window) {
            if(_mimas->locked_cursor_window == window) {
                lock_cursor_to_window(window);
            } else if(_mimas->clipped_cursor_window == window) {
                Mimas_Rect const client = { 0, 0, width, height };
                Mimas_Rect const clip_rect = crop_rect(&_mimas->cursor_clip_region, &client);
                clip_cursor_to_window(window, &clip_rect);
            }
        }
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

void mimas_platform_clip_cursor(Mimas_Window* const window, Mimas_Rect const* const region) {
    Mimas_Internal* const _mimas = _mimas_get_mimas_internal();
    if(_mimas->cursor_locked) {
        return;
    }
    
    if(region) {
        Mimas_Rect corrected_region = *region;
        corrected_region.left = min_i32(region->left, region->right);
        corrected_region.top = min_i32(region->top, region->bottom);
        _mimas->cursor_clip_region = corrected_region;
        if(_mimas->active_window == window) {
            // Correct the region so that it doesn't extend outside the client area
            Mimas_Rect const client = get_client_rect(window); 
            Mimas_Rect const clip_region = crop_rect(&corrected_region, (Mimas_Rect*)&client);
            clip_cursor_to_window(window, clip_region);
        }

        _mimas->cursor_clipped = mimas_true;
        _mimas->clipped_cursor_window = window;
    } else {
        if(_mimas->active_window == _mimas->clipped_cursor_window) {
            ClipCursor(NULL);
        }

        _mimas->cursor_clipped = mimas_false;
        _mimas->clipped_cursor_window = NULL;
    }
}

void mimas_platform_lock_cursor() {
    Mimas_Internal* const _mimas = _mimas_get_mimas_internal();
    if(_mimas->cursor_locked || _mimas->cursor_clipped) {
        return;
    }

    _mimas->cursor_locked = mimas_true;
    // Reset cursor position
    _mimas->cursor_virtual_pos_x = 0;
    _mimas->cursor_virtual_pos_y = 0;

    mimas_i32 x, y;
    mimas_platform_get_cursor_pos(&x, &y);
    RECT clip_region = {.left = x, .right = x + 1, .top = y, .bottom = y + 1};
    ClipCursor(&clip_region);
}

void mimas_platform_unlock_cursor() {
    Mimas_Internal* const _mimas = _mimas_get_mimas_internal();
    if(!_mimas->cursor_locked || _mimas->locked_cursor_window) {
        return;
    }

    ClipCursor(NULL);
    _mimas->cursor_locked = mimas_false;
}

void mimas_platform_lock_cursor_to_window(Mimas_Window* const window) {
    Mimas_Internal* const _mimas = _mimas_get_mimas_internal();
    if(_mimas->cursor_locked || _mimas->cursor_clipped) {
        return;
    }

    _mimas->cursor_locked = true;
    _mimas->locked_cursor_window = window;
    // Reset cursor position
    _mimas->cursor_virtual_pos_x = 0;
    _mimas->cursor_virtual_pos_y = 0;
    if(_mimas->active_window == _mimas->locked_cursor_window) {
        lock_cursor_to_window(window);
    }
}

void mimas_platform_unlock_cursor_from_window() {
    Mimas_Internal* const _mimas = _mimas_get_mimas_internal();
    if(!_mimas->cursor_locked || !_mimas->locked_cursor_window) {
        return;
    }

    if(_mimas->active_window == _mimas->locked_cursor_window) {
        ClipCursor(NULL);
    }

    _mimas->cursor_locked = mimas_false;
    _mimas->locked_cursor_window = NULL;
}
