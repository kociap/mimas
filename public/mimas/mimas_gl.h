#ifndef MIMAS_MIMAS_GL_H_INCLUDE
#define MIMAS_MIMAS_GL_H_INCLUDE

#include <mimas/mimas.h>

EXTERN_C_BEGIN

typedef struct Mimas_GL_Context Mimas_GL_Context;

typedef enum {
    MIMAS_GL_CORE_PROFILE,
    MIMAS_GL_COMPATIBILITY_PROFILE,
} Mimas_GL_Profile;

MIMAS_API mimas_bool mimas_init_with_gl();

MIMAS_API Mimas_GL_Context* mimas_create_gl_context(mimas_i32 version_major, mimas_i32 version_minor, Mimas_GL_Profile profile);
MIMAS_API void mimas_destroy_gl_context(Mimas_GL_Context* ctx);
MIMAS_API mimas_bool mimas_make_context_current(Mimas_Window* const window, Mimas_GL_Context* const ctx);

EXTERN_C_END

#endif // !MIMAS_MIMAS_GL_H_INCLUDE
