#ifndef MIMAS_PLATFORM_GL_H_INCLUDE
#define MIMAS_PLATFORM_GL_H_INCLUDE

#include <mimas/mimas_gl.h>

mimas_bool mimas_platform_init_with_gl();
void mimas_platform_terminate_with_gl();

Mimas_GL_Context* mimas_platform_create_gl_context(mimas_i32 major, mimas_i32 minor, Mimas_GL_Profile profile);
void mimas_platform_destroy_gl_context(Mimas_GL_Context* ctx);
mimas_bool mimas_platform_make_context_current(Mimas_Window* const window, Mimas_GL_Context* const ctx);

#endif // !MIMAS_PLATFORM_GL_H_INCLUDE
