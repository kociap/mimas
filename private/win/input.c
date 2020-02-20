#include <platform.h>
#include <win/platform.h>
#include <mimas/mimas.h>

void mimas_platform_get_cursor_pos(mimas_i32* const x, mimas_i32* const y) {
    POINT cursor = {0, 0};
    GetCursorPos(&cursor);
    *x = cursor.x;
    *y = cursor.y;
}

Mimas_Mouse_Button_Action mimas_platform_get_mouse_button(Mimas_Mouse_Button button) {
    Mimas_Win_Platform const* platform = (Mimas_Win_Platform const*)_mimas_get_mimas_internal()->platform;
    return platform->mouse_state[button] ? MIMAS_MOUSE_BUTTON_PRESS : MIMAS_MOUSE_BUTTON_RELEASE;
}  