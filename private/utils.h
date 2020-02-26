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
        cropped.left = min_i32(max_i32(target->left, source->left), target->right),
        cropped.right = min_i32(max_i32(target->left, source->right), target->right),
        cropped.top = min_i32(max_i32(target->top, source->top), target->bottom),
        cropped.bottom = min_i32(max_i32(target->top, source->bottom), target->bottom)
    };
    return cropped;
}

#endif // !MIMAS_UTILS_H_INCLUDE
