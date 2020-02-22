#include <platform.h>
#include <win/platform.h>
#include <mimas/mimas.h>

void mimas_platform_get_cursor_pos(mimas_i32* const x, mimas_i32* const y) {
    POINT cursor = {0, 0};
    GetCursorPos(&cursor);
    *x = cursor.x;
    *y = cursor.y;
}

Mimas_Key_Action mimas_platform_get_key(Mimas_Key const key) {
    Mimas_Win_Platform const* platform = (Mimas_Win_Platform const*)_mimas_get_mimas_internal()->platform;
    return platform->key_state[key];
}
