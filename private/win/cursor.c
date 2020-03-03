#include <win/platform.h>
#include <internal.h>

#include <stdlib.h>

static LPWSTR mimas_cursor_to_win_cursor(Mimas_Standard_Cursor const cursor) {
    switch(cursor) {
        case MIMAS_CURSOR_ARROW:
            return IDC_ARROW;
        
        case MIMAS_CURSOR_RESIZE_LEFT:
        case MIMAS_CURSOR_RESIZE_RIGHT:
            return IDC_SIZEWE;

        case MIMAS_CURSOR_RESIZE_BOTTOM:
        case MIMAS_CURSOR_RESIZE_TOP:
            return IDC_SIZENS;

        case MIMAS_CURSOR_RESIZE_TOP_LEFT:
        case MIMAS_CURSOR_RESIZE_BOTTOM_RIGHT:
            return IDC_SIZENESW;

        case MIMAS_CURSOR_RESIZE_TOP_RIGHT:
        case MIMAS_CURSOR_RESIZE_BOTTOM_LEFT:
            return IDC_SIZENWSE;

        default:
            return IDC_ARROW;
    }
}

Mimas_Cursor* mimas_platform_create_standard_cursor(Mimas_Standard_Cursor const cursor) {
    return (Mimas_Cursor*)LoadImageW(NULL, mimas_cursor_to_win_cursor(cursor), IMAGE_CURSOR, 0, 0, LR_SHARED | LR_DEFAULTSIZE);
}

void mimas_platform_destroy_cursor(Mimas_Cursor* cursor) {
    DestroyCursor((HCURSOR)cursor);
}

void mimas_platform_set_cursor(Mimas_Window* window, Mimas_Cursor* cursor) {
    window->cursor = cursor;
}
