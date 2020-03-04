#include <mimas/mimas_gl.h>
#include <internal.h>
#include <platform_gl.h>
#include <platform.h>

mimas_bool mimas_init_with_gl(void) {
    _mimas_init_internal(MIMAS_BACKEND_GL);
    mimas_bool const res = mimas_platform_init(MIMAS_BACKEND_GL);
    if(!res) {
        _mimas_terminate_internal();
    }
    return res;
}

Mimas_GL_Context* mimas_create_gl_context(mimas_i32 const version_major, mimas_i32 const version_minor, Mimas_GL_Profile const profile) {
    return mimas_platform_create_gl_context(version_major, version_minor, profile);
}

void mimas_destroy_gl_context(Mimas_GL_Context* const ctx) {
    mimas_platform_destroy_gl_context(ctx);
}

mimas_bool mimas_make_context_current(Mimas_Window* const window, Mimas_GL_Context* const ctx) {
    return mimas_platform_make_context_current(window, ctx);
}

void mimas_swap_buffers(Mimas_Window* const window) {
    mimas_platform_swap_buffers(window);
}

void mimas_set_swap_interval(mimas_i32 const interval) {
    mimas_platform_set_swap_interval(interval);
}

mimas_i32 mimas_get_swap_interval(void) {
    return mimas_platform_get_swap_interval();
}
