#include <platform_gl.h>
#include <win/platform.h>
#include <win/wgl.h>
#include <internal.h>

#include <wingdi.h>

Mimas_GL_Context* mimas_platform_create_gl_context(mimas_i32 const major, mimas_i32 const minor, Mimas_GL_Profile const profile) {
    Mimas_Win_Platform* const platform = (Mimas_Win_Platform*)_mimas_get_mimas_internal()->platform;
    Mimas_Win_Window* const native_window = (Mimas_Win_Window*)platform->dummy_window->native_window;
    HDC const hdc = GetDC(native_window->handle);
    int gl_profile = (profile == MIMAS_GL_COMPATIBILITY_PROFILE ? WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB : WGL_CONTEXT_CORE_PROFILE_BIT_ARB);
    int const attrib_list[] = {WGL_CONTEXT_MAJOR_VERSION_ARB, major, WGL_CONTEXT_MINOR_VERSION_ARB, minor, WGL_CONTEXT_PROFILE_MASK_ARB, gl_profile, 0};
    HGLRC const hglrc = wglCreateContextAttribsARB(hdc, NULL, attrib_list);
    return (Mimas_GL_Context*)hglrc;
}

void mimas_platform_destroy_gl_context(Mimas_GL_Context* const ctx) {
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext((HGLRC)ctx);
}

mimas_bool mimas_platform_make_context_current(Mimas_Window* const window, Mimas_GL_Context* const ctx) {
    Mimas_Win_Window* const native_window = (Mimas_Win_Window*)window->native_window;
    HDC const hdc = GetDC(native_window->handle);
    if(!wglMakeCurrent(hdc, (HGLRC)ctx)) {
        // TODO: Error
        return mimas_false;
    }

    return mimas_true;
}
