#include <mimas/mimas.h>
#include <platform.h>
#include <win/platform.h>

#include <internal.h>

#include <hidusage.h>
#include <windows.h>

void _mimas_install_input_listener()
{
  Mimas_Internal* _mimas = _mimas_get_mimas_internal();
  Mimas_Win_Platform* platform = (Mimas_Win_Platform*)_mimas->platform;
  Mimas_Win_Window* dummy_window =
    (Mimas_Win_Window*)platform->dummy_window->native_window;
  HWND dummy_hwnd = dummy_window->handle;

  mimas_u32 flags = 0;
  if(platform->enable_raw_input_sink) {
    flags |= RIDEV_INPUTSINK;
  }

  RAWINPUTDEVICE rid[] = {
    {.usUsagePage = HID_USAGE_PAGE_GENERIC,
     .usUsage = HID_USAGE_GENERIC_MOUSE,
     .dwFlags = flags,
     .hwndTarget = dummy_hwnd},
    {.usUsagePage = HID_USAGE_PAGE_GENERIC,
     .usUsage = HID_USAGE_GENERIC_KEYBOARD,
     .dwFlags = flags,
     .hwndTarget = dummy_hwnd},
    {.usUsagePage = HID_USAGE_PAGE_GENERIC,
     .usUsage = HID_USAGE_GENERIC_JOYSTICK,
     .dwFlags = flags,
     .hwndTarget = dummy_hwnd},
    {.usUsagePage = HID_USAGE_PAGE_GENERIC,
     .usUsage = HID_USAGE_GENERIC_GAMEPAD,
     .dwFlags = flags,
     .hwndTarget = dummy_hwnd},
  };
  RegisterRawInputDevices(rid, 4, sizeof(RAWINPUTDEVICE));
}

void _mimas_uninstall_input_listener()
{
  RAWINPUTDEVICE rid_old[] = {{.usUsagePage = HID_USAGE_PAGE_GENERIC,
                               .usUsage = HID_USAGE_GENERIC_MOUSE,
                               .dwFlags = RIDEV_REMOVE,
                               .hwndTarget = NULL},
                              {.usUsagePage = HID_USAGE_PAGE_GENERIC,
                               .usUsage = HID_USAGE_GENERIC_KEYBOARD,
                               .dwFlags = RIDEV_REMOVE,
                               .hwndTarget = NULL},
                              {.usUsagePage = HID_USAGE_PAGE_GENERIC,
                               .usUsage = HID_USAGE_GENERIC_JOYSTICK,
                               .dwFlags = RIDEV_REMOVE,
                               .hwndTarget = NULL},
                              {.usUsagePage = HID_USAGE_PAGE_GENERIC,
                               .usUsage = HID_USAGE_GENERIC_GAMEPAD,
                               .dwFlags = RIDEV_REMOVE,
                               .hwndTarget = NULL}};

  RegisterRawInputDevices(rid_old, 4, sizeof(RAWINPUTDEVICE));
}

void mimas_set_cursor_pos(mimas_i32 const x, mimas_i32 const y)
{
  SetCursorPos(x, y);
}

void mimas_get_cursor_pos(mimas_i32* const x, mimas_i32* const y)
{
  POINT p = {0};
  GetCursorPos(&p);
  *x = p.x;
  *y = p.y;
}

Mimas_Key_Action mimas_get_key(Mimas_Key const key)
{
  if(key == MIMAS_KEY_UNKNOWN) {
    return MIMAS_KEY_RELEASE;
  } else {
    Mimas_Win_Platform const* platform =
      (Mimas_Win_Platform const*)_mimas_get_mimas_internal()->platform;
    return platform->key_state[key];
  }
}
