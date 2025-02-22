#pragma once

#include <mimas/mimas.h>

#define UNUSED(v) ((void)v)
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))

mimas_u8 _mimas_max_u8(mimas_u8 x, mimas_u8 y);
mimas_i32 _mimas_min_i32(mimas_i32 x, mimas_i32 y);
mimas_i32 _mimas_max_i32(mimas_i32 x, mimas_i32 y);

Mimas_Rect _mimas_crop_rect(Mimas_Rect const* source, Mimas_Rect const* target);

// _mimas_utf8_to_utf16
// Converts a UTF-8 encoded string contained in buffer_utf8 to a UTF-16 encoded string.
// This function calculates the size required to fit the converted string
// and allocated a buffer of that size which it later returns.
//
// Parameters:
// buffer_utf8 - UTF-8 encoded string to be converted.
//       count - is the number of bytes in buffer_utf8 to be converted. If count is -1,
//               the function will convert all charcaters up until and including null-terminator.
//
// Returns:
// A null-terminated buffer containing UTF-16 encoded string.
//
mimas_char16* _mimas_utf8_to_utf16(mimas_char8 const* buffer_utf8,
                                   mimas_i64 count);
