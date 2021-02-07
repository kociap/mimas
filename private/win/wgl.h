#ifndef MIMAS_WIN_WGL_H_INCLUDE
#define MIMAS_WIN_WGL_H_INCLUDE

#define NOMINMAX 1
#define WIN32_LEAN_AND_MEAN 1

#define _UNICODE 1
#define UNICODE 1

#include <Windows.h>
#include <mimas/mimas.h>

// Assumes pixel format is already set on the given HDC.
mimas_bool mimas_load_wgl(HDC hdc);
void mimas_unload_wgl(void);

typedef char const* (*PFN_wglGetExtensionsStringARB)(HDC hdc);
extern PFN_wglGetExtensionsStringARB mimas_wglGetExtensionsStringARB;
#define wglGetExtensionsStringARB mimas_wglGetExtensionsStringARB;


typedef HGLRC (*PFN_wglCreateContext)(HDC hdc);
typedef BOOL (*PFN_wglDeleteContext)(HGLRC);
typedef HDC(*PFN_wglGetCurrentDC)(void);
typedef HGLRC(*PFN_wglGetCurrentContext)(void);
typedef BOOL (*PFN_wglMakeCurrent)(HDC hdc, HGLRC hglrc);
typedef BOOL (*PFN_wglShareLists)(HGLRC hglrc1, HGLRC hglrc2);
typedef void (*PFN_wglSwapBuffers)(HDC hdc);
extern PFN_wglCreateContext mimas_wglCreateContext;
extern PFN_wglDeleteContext mimas_wglDeleteContext;
extern PFN_wglGetCurrentDC mimas_wglGetCurrentDC;
extern PFN_wglGetCurrentContext mimas_wglGetCurrentContext;
extern PFN_wglMakeCurrent mimas_wglMakeCurrent;
extern PFN_wglShareLists mimas_wglShareLists;
extern PFN_wglSwapBuffers mimas_wglSwapBuffers;
#define wglCreateContext mimas_wglCreateContext
#define wglDeleteContext mimas_wglDeleteContext
#define wglGetCurrentDC mimas_wglGetCurrentDC
#define wglGetCurrentContext mimas_wglGetCurrentContext
#define wglMakeCurrent mimas_wglMakeCurrent
#define wglShareLists mimas_wglShareLists
#define wglSwapBuffers mimas_wglSwapBuffers


// WGL_ARB_create_context
#define WGL_CONTEXT_MAJOR_VERSION_ARB               0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB               0x2092
#define WGL_CONTEXT_PROFILE_MASK_ARB                0x9126
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB            0x00000001
#define WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB   0x00000002
#define WGL_CONTEXT_FLAGS_ARB                       0x2094
#define WGL_CONTEXT_DEBUG_BIT_ARB                   0x00000001
#define WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB      0x00000002
#define WGL_CONTEXT_LAYER_PLANE_ARB                 0x2093
#define WGL_CONTEXT_FLAGS_ARB                       0x2094
#define ERROR_INVALID_VERSION_ARB                   0x2095
#define ERROR_INVALID_PROFILE_ARB                   0x2096
typedef HGLRC (*PFN_wglCreateContextAttribsARB)(HDC hDC, HGLRC hShareContext, int const* attribList);
extern PFN_wglCreateContextAttribsARB mimas_wglCreateContextAttribsARB;
#define wglCreateContextAttribsARB mimas_wglCreateContextAttribsARB


// WGL_ARB_pixel_format
#define WGL_NUMBER_PIXEL_FORMATS_ARB      0x2000
#define WGL_DRAW_TO_WINDOW_ARB            0x2001
#define WGL_DRAW_TO_BITMAP_ARB            0x2002
#define WGL_ACCELERATION_ARB              0x2003
#define WGL_NEED_PALETTE_ARB              0x2004
#define WGL_NEED_SYSTEM_PALETTE_ARB       0x2005
#define WGL_SWAP_LAYER_BUFFERS_ARB        0x2006
#define WGL_SWAP_METHOD_ARB               0x2007
#define WGL_NUMBER_OVERLAYS_ARB           0x2008
#define WGL_NUMBER_UNDERLAYS_ARB          0x2009
#define WGL_TRANSPARENT_ARB               0x200A
#define WGL_TRANSPARENT_RED_VALUE_ARB     0x2037
#define WGL_TRANSPARENT_GREEN_VALUE_ARB   0x2038
#define WGL_TRANSPARENT_BLUE_VALUE_ARB    0x2039
#define WGL_TRANSPARENT_ALPHA_VALUE_ARB   0x203A
#define WGL_TRANSPARENT_INDEX_VALUE_ARB   0x203B
#define WGL_SHARE_DEPTH_ARB               0x200C
#define WGL_SHARE_STENCIL_ARB             0x200D
#define WGL_SHARE_ACCUM_ARB               0x200E
#define WGL_SUPPORT_GDI_ARB               0x200F
#define WGL_SUPPORT_OPENGL_ARB            0x2010
#define WGL_DOUBLE_BUFFER_ARB             0x2011
#define WGL_STEREO_ARB                    0x2012
#define WGL_PIXEL_TYPE_ARB                0x2013
#define WGL_COLOR_BITS_ARB                0x2014
#define WGL_RED_BITS_ARB                  0x2015
#define WGL_RED_SHIFT_ARB                 0x2016
#define WGL_GREEN_BITS_ARB                0x2017
#define WGL_GREEN_SHIFT_ARB               0x2018
#define WGL_BLUE_BITS_ARB                 0x2019
#define WGL_BLUE_SHIFT_ARB                0x201A
#define WGL_ALPHA_BITS_ARB                0x201B
#define WGL_ALPHA_SHIFT_ARB               0x201C
#define WGL_ACCUM_BITS_ARB                0x201D
#define WGL_ACCUM_RED_BITS_ARB            0x201E
#define WGL_ACCUM_GREEN_BITS_ARB          0x201F
#define WGL_ACCUM_BLUE_BITS_ARB           0x2020
#define WGL_ACCUM_ALPHA_BITS_ARB          0x2021
#define WGL_DEPTH_BITS_ARB                0x2022
#define WGL_STENCIL_BITS_ARB              0x2023
#define WGL_AUX_BUFFERS_ARB               0x2024
#define WGL_NO_ACCELERATION_ARB           0x2025
#define WGL_GENERIC_ACCELERATION_ARB      0x2026
#define WGL_FULL_ACCELERATION_ARB         0x2027
#define WGL_SWAP_EXCHANGE_ARB             0x2028
#define WGL_SWAP_COPY_ARB                 0x2029
#define WGL_SWAP_UNDEFINED_ARB            0x202A
#define WGL_TYPE_RGBA_ARB                 0x202B
#define WGL_TYPE_COLORINDEX_ARB           0x202C
typedef BOOL (WINAPI* PFN_wglGetPixelFormatAttribivARB)(HDC hdc, int iPixelFormat, int iLayerPlane, UINT nAttributes, int const* piAttributes, int* piValues);
typedef BOOL (WINAPI* PFN_wglGetPixelFormatAttribfvARB)(HDC hdc, int iPixelFormat, int iLayerPlane, UINT nAttributes, int const* piAttributes, FLOAT* pfValues);
typedef BOOL (WINAPI* PFN_wglChoosePixelFormatARB)(HDC hdc, int const* piAttribIList, FLOAT const* pfAttribFList, UINT nMaxFormats, int* piFormats, UINT* nNumFormats);
extern PFN_wglGetPixelFormatAttribivARB mimas_wglGetPixelFormatAttribivARB;
extern PFN_wglGetPixelFormatAttribfvARB mimas_wglGetPixelFormatAttribfvARB;
extern PFN_wglChoosePixelFormatARB mimas_wglChoosePixelFormatARB;
#define wglGetPixelFormatAttribivARB mimas_wglGetPixelFormatAttribivARB
#define wglGetPixelFormatAttribfvARB mimas_wglGetPixelFormatAttribfvARB
#define wglChoosePixelFormatARB mimas_wglChoosePixelFormatARB


// WGL_EXT_swap_control
typedef BOOL (WINAPI* PFN_wglSwapIntervalEXT)(int interval);
typedef int (WINAPI* PFN_wglGetSwapIntervalEXT)(void);
extern PFN_wglSwapIntervalEXT mimas_wglSwapIntervalEXT;
extern PFN_wglGetSwapIntervalEXT mimas_wglGetSwapIntervalEXT;
#define wglSwapIntervalEXT mimas_wglSwapIntervalEXT
#define wglGetSwapIntervalEXT mimas_wglGetSwapIntervalEXT

#endif // !MIMAS_WIN_WGL_H_INCLUDE
