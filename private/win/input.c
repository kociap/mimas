#include <platform.h>
#include <win/platform.h>
#include <mimas/mimas.h>

void mimas_platform_get_cursor_pos(mimas_i32* const x, mimas_i32* const y) {
    POINT cursor = {0, 0};
    GetCursorPos(&cursor);
    *x = cursor.x;
    *y = cursor.y;
}
