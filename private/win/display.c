#include <win/platform.h>
#include <internal.h>
#include <utils.h>

#include <stdlib.h>

void _mimas_free_displays(Mimas_Display** displays, mimas_i64 const count) {
    for(mimas_u32 i = 0; i < count; i += 1) {
        Mimas_Display* display = displays[i];
        free(display->native_display);
        free(display);
    }
    free(displays);
}

typedef struct Enum_Monitor_Data {
    HMONITOR handle;
    DISPLAY_DEVICE* adapter_device;
} Enum_Monitor_Data;

static BOOL enum_monitor_callback(HMONITOR hmonitor, HDC hdc, LPRECT rect, LPARAM lparam) {
    UNUSED(hdc); UNUSED(rect);
    Enum_Monitor_Data* const data = (Enum_Monitor_Data*)lparam;
    MONITORINFOEXW info = { .cbSize = sizeof(MONITORINFOEX) };
    if(GetMonitorInfoW(hmonitor, (MONITORINFO*)&info)) {
        if(wcscmp(data->adapter_device->DeviceName, info.szDevice) == 0) {
            data->handle = hmonitor;
            return FALSE;
        }
    }

    return TRUE;
}

Mimas_Display** _mimas_get_connected_displays(mimas_i64* const count) {
    mimas_u32 display_array_size = 128;
    Mimas_Display** displays = (Mimas_Display**)malloc(display_array_size * sizeof(Mimas_Display*));
    if(!displays) {
        *count = 0;
        return NULL;
    }
    memset(displays, 0, sizeof(Mimas_Display*) * display_array_size);

    mimas_u32 display_count = 0;
    DISPLAY_DEVICE adapter_device = { .cb = sizeof(DISPLAY_DEVICE) };
    for(mimas_u32 adapter_index = 0; EnumDisplayDevicesW(NULL, adapter_index, &adapter_device, 0); adapter_index += 1) {
        DISPLAY_DEVICE display_device = { .cb = sizeof(DISPLAY_DEVICE) };
        for(mimas_u32 display_index = 0; EnumDisplayDevicesW(adapter_device.DeviceName, display_index, &display_device, 0); display_index += 1) {
            if(!(display_device.StateFlags & DISPLAY_DEVICE_ATTACHED_TO_DESKTOP)) {
                continue;
            }

            if(display_count == display_array_size) {
                Mimas_Display** r = realloc(displays, 2 * display_array_size * sizeof(Mimas_Display*));
                if(!r) {
                    _mimas_free_displays(displays, display_count);
                    *count = 0;
                    // TODO: Error
                    return NULL;
                }

                displays = r;
                display_array_size *= 2;
            }

            Mimas_Display* const display = (Mimas_Display*)malloc(sizeof(Mimas_Display));
            Mimas_Win_Display* const native_display = (Mimas_Win_Display*)malloc(sizeof(Mimas_Win_Display));
            if(!display || !native_display) {
                free(display);
                free(native_display);
                _mimas_free_displays(displays, display_count);
                *count = 0;
                // TODO: Error
                return NULL;
            }
            memset(display, 0, sizeof(Mimas_Display));
            memset(native_display, 0, sizeof(Mimas_Win_Display));
            WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, display_device.DeviceName, -1, (char*)native_display->display_name, ARRAY_SIZE(native_display->display_name), NULL, NULL);
            WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, adapter_device.DeviceName, -1, (char*)native_display->adapter_name, ARRAY_SIZE(native_display->adapter_name), NULL, NULL);
            display->native_display = native_display;
            if(display_device.StateFlags & DISPLAY_DEVICE_PRIMARY_DEVICE) {
                memmove(displays + 1, displays, sizeof(Mimas_Display*) * display_count);
                displays[0] = display;
            } else {
                displays[display_count] = display;
            }
            display_count += 1;

            Enum_Monitor_Data enum_monitor_data = { .adapter_device = &adapter_device };
            EnumDisplayMonitors(NULL, NULL, enum_monitor_callback, (LPARAM)&enum_monitor_data);
            if(!enum_monitor_data.handle) {
                _mimas_free_displays(displays, display_count);
                *count = 0;
                // TODO: Error
                return NULL;
            }

            native_display->hmonitor = enum_monitor_data.handle;

            memset(&display_device, 0, sizeof(DISPLAY_DEVICE));
            display_device.cb = sizeof(DISPLAY_DEVICE);
        }

        memset(&adapter_device, 0, sizeof(DISPLAY_DEVICE));
        adapter_device.cb = sizeof(DISPLAY_DEVICE);
    }

    *count = display_count;
    return displays;
}

Mimas_Display_Settings mimas_get_display_settings(Mimas_Display* display) {
    Mimas_Win_Display* const native_display = (Mimas_Win_Display*)display->native_display;
    WCHAR device_name[32] = {0};
    MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, (char*)native_display->adapter_name, -1, device_name, 32);
    DEVMODEW devmode = {0};
    EnumDisplaySettingsW(device_name, ENUM_CURRENT_SETTINGS, &devmode);
    Mimas_Display_Settings settings = { .width = devmode.dmPelsWidth, .height = devmode.dmPelsHeight, .refresh_rate = devmode.dmDisplayFrequency };
    return settings;
}
