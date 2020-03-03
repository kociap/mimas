#ifndef MIMAS_UTILS_H_INCLUDE
#define MIMAS_UTILS_H_INCLUDE

#include <mimas/mimas.h>

#define UNUSED(v) ((void)v)
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))

inline mimas_i32 min_i32(mimas_i32 x, mimas_i32 y) {
    return x < y ? x : y;
}

inline mimas_i32 max_i32(mimas_i32 x, mimas_i32 y) {
    return x > y ? x : y;
}

inline Mimas_Rect crop_rect(Mimas_Rect const* const source, Mimas_Rect const* const target) {
    Mimas_Rect cropped = {
        .left = min_i32(max_i32(target->left, source->left), target->right),
        .right = min_i32(max_i32(target->left, source->right), target->right),
        .top = min_i32(max_i32(target->top, source->top), target->bottom),
        .bottom = min_i32(max_i32(target->top, source->bottom), target->bottom)
    };
    return cropped;
}

#endif // !MIMAS_UTILS_H_INCLUDE
