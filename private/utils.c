#include <utils.h>

#include <stdlib.h>
#include <string.h>

mimas_u8 _mimas_max_u8(mimas_u8 x, mimas_u8 y) {
    return x > y ? x : y;
}

mimas_i32 _mimas_min_i32(mimas_i32 x, mimas_i32 y) {
    return x < y ? x : y;
}

mimas_i32 _mimas_max_i32(mimas_i32 x, mimas_i32 y) {
    return x > y ? x : y;
}

Mimas_Rect _mimas_crop_rect(Mimas_Rect const* const source, Mimas_Rect const* const target) {
    Mimas_Rect cropped = {
        .left = _mimas_min_i32(_mimas_max_i32(target->left, source->left), target->right),
        .right = _mimas_min_i32(_mimas_max_i32(target->left, source->right), target->right),
        .top = _mimas_min_i32(_mimas_max_i32(target->top, source->top), target->bottom),
        .bottom = _mimas_min_i32(_mimas_max_i32(target->top, source->bottom), target->bottom)
    };
    return cropped;
}

// _mimas_popcount_u8
// Counts the number of set bits in v.
//
static mimas_u8 _mimas_popcount_u8(mimas_u8 v) {
    v = (v & 0x55) + ((v >> 1) & 0x55);
    v = (v & 0x33) + ((v >> 2) & 0x33);
    v = (v & 0x0F) + ((v >> 4) & 0x0F);
    return v;
}

// _mimas_clz_u8
// Counts leading zeros.
//
static mimas_u8 _mimas_clz_u8(mimas_u8 v) {
    v |= (v >> 1);
    v |= (v >> 2);
    v |= (v >> 4);
    return 8 - _mimas_popcount_u8(v);
}

typedef struct Codepoint_Conversion_Result {
        mimas_char32 codepoint;
        mimas_i32 bytes_read;
} Codepoint_Conversion_Result;

static Codepoint_Conversion_Result utf8_bytes_to_codepoint(mimas_char8 const* bytes) {
    mimas_char8 const leading_byte = *bytes;
    // Copied from get_byte_count_from_utf8_leading_byte because we need the leading zeroes.
    mimas_u8 const leading_zeros = _mimas_clz_u8((mimas_u8)~leading_byte);
    mimas_i32 const byte_count = _mimas_max_u8((mimas_u8)1, leading_zeros);
    mimas_char32 codepoint = leading_byte & (0xFF >> (leading_zeros + 1));
    for(mimas_i32 i = 1; i < byte_count; ++i) {
        codepoint <<= 6;
        codepoint |= bytes[i] & 0x3F;
    }
    return (Codepoint_Conversion_Result){codepoint, byte_count};
}

mimas_char16* _mimas_utf8_to_utf16(mimas_char8 const* const buffer_utf8, mimas_i64 const count) {
    mimas_i64 bytes_needed = 0;
    {
        mimas_char8 const* buffer_utf8_iter = buffer_utf8;
        mimas_i64 bytes_read = 0;
        while(mimas_true) {
            Codepoint_Conversion_Result const conv = utf8_bytes_to_codepoint(buffer_utf8_iter);
            buffer_utf8_iter += conv.bytes_read;
            bytes_read += conv.bytes_read;
            // U+0000 to U+D7FF and U+E000 to U+FFFF are encoded as single 16bit chars.
            mimas_bool const is_surrogate_pair = !(conv.codepoint <= 0xD7FF || (conv.codepoint >= 0xE000 && conv.codepoint <= 0xFFFF));
            bytes_needed += 2 * (is_surrogate_pair + 1);
            if((count == -1 && conv.codepoint == U'\0') || (count != -1 && bytes_read >= count)) {
                break;
            }
        }
    }

    // Allocate additional memory for a null-terminator
    if(count != -1) {
        bytes_needed += 2;
    }

    mimas_char16* const buffer_utf16 = (mimas_char16*)malloc(bytes_needed);
    if(buffer_utf16 == NULL) {
        return NULL;
    }

    // Full memset is a little wasteful
    memset(buffer_utf16, 0, bytes_needed);

    mimas_char8 const* buffer_utf8_iter = buffer_utf8;
    mimas_char16* buffer_utf16_iter = buffer_utf16;
    for(mimas_i64 bytes_read = 0; mimas_true;) {
        Codepoint_Conversion_Result const conv = utf8_bytes_to_codepoint(buffer_utf8_iter);
        buffer_utf8_iter += conv.bytes_read;
        bytes_read += conv.bytes_read;
        // U+0000 to U+D7FF and U+E000 to U+FFFF are encoded as single 16bit chars, which we expect to be the more common case.
        if(conv.codepoint <= 0xD7FF || (conv.codepoint >= 0xE000 && conv.codepoint <= 0xFFFF)) {
            *buffer_utf16_iter = conv.codepoint;
            buffer_utf16_iter += 1;
        } else {
            mimas_char16 const high_surrogate = 0xD800 + ((conv.codepoint - 0x10000) >> 10 & 0x3FF);
            mimas_char16 const low_surrogate = 0xDC00 + ((conv.codepoint - 0x10000) & 0x3FF);
            buffer_utf16_iter[0] = high_surrogate;
            buffer_utf16_iter[1] = low_surrogate;
            buffer_utf16_iter += 2;
        }

        if((count == -1 && conv.codepoint == U'\0') || (count != -1 && bytes_read >= count)) {
            break;
        }
    }

    return buffer_utf16;
}
