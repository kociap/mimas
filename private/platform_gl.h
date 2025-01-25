#ifndef MIMAS_PLATFORM_GL_H_INCLUDE
#define MIMAS_PLATFORM_GL_H_INCLUDE

#include <mimas/mimas_gl.h>

mimas_bool mimas_platform_init_gl_backend(void);
void mimas_platform_terminate_gl_backend(void);

Mimas_GL_Context* mimas_platform_create_gl_context(mimas_i32 major,
                                                   mimas_i32 minor,
                                                   Mimas_GL_Profile profile);
void mimas_platform_destroy_gl_context(Mimas_GL_Context* ctx);
mimas_bool mimas_platform_make_context_current(Mimas_Window* const window,
                                               Mimas_GL_Context* const ctx);

void mimas_platform_swap_buffers(Mimas_Window*);
void mimas_platform_set_swap_interval(mimas_i32);
mimas_i32 mimas_platform_get_swap_interval(void);

#endif // !MIMAS_PLATFORM_GL_H_INCLUDE
