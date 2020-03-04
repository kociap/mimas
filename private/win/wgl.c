#include <win/wgl.h>

#include <wingdi.h>

static HMODULE opengl_module = NULL;

typedef void* (*PFN_wlgGetProcAddress)(char const*);
static PFN_wlgGetProcAddress mimas_wglGetProcAddress = NULL;
#define wglGetProcAddress mimas_wglGetProcAddress

PFN_wglCreateContext mimas_wglCreateContext = NULL;
PFN_wglDeleteContext mimas_wglDeleteContext = NULL;
PFN_wglGetCurrentDC mimas_wglGetCurrentDC = NULL;
PFN_wglGetCurrentContext mimas_wglGetCurrentContext = NULL;
PFN_wglMakeCurrent mimas_wglMakeCurrent = NULL;
PFN_wglShareLists mimas_wglShareLists = NULL;
PFN_wglSwapBuffers mimas_wglSwapBuffers = NULL;

PFN_wglGetExtensionsStringARB mimas_wglGetExtensionsStringARB = NULL;
PFN_wglCreateContextAttribsARB mimas_wglCreateContextAttribsARB = NULL;
PFN_wglGetPixelFormatAttribivARB mimas_wglGetPixelFormatAttribivARB = NULL;
PFN_wglGetPixelFormatAttribfvARB mimas_wglGetPixelFormatAttribfvARB = NULL;
PFN_wglChoosePixelFormatARB mimas_wglChoosePixelFormatARB = NULL;
PFN_wglSwapIntervalEXT mimas_wglSwapIntervalEXT = NULL;
PFN_wglGetSwapIntervalEXT mimas_wglGetSwapIntervalEXT = NULL;

mimas_bool mimas_load_wgl(HDC const hdc) {
    opengl_module = LoadLibraryW(L"opengl32.dll");
    if(opengl_module) {
        mimas_wglGetProcAddress = (PFN_wlgGetProcAddress)GetProcAddress(opengl_module, "wglGetProcAddress");
        mimas_wglCreateContext = (PFN_wglCreateContext)GetProcAddress(opengl_module, "wglCreateContext");
        mimas_wglDeleteContext = (PFN_wglDeleteContext)GetProcAddress(opengl_module, "wglDeleteContext");
        mimas_wglGetCurrentDC = (PFN_wglGetCurrentDC)GetProcAddress(opengl_module, "wglGetCurrentDC");
        mimas_wglGetCurrentContext = (PFN_wglGetCurrentContext)GetProcAddress(opengl_module, "wglGetCurrentContext");
        mimas_wglMakeCurrent = (PFN_wglMakeCurrent)GetProcAddress(opengl_module, "wglMakeCurrent");
        mimas_wglShareLists = (PFN_wglShareLists)GetProcAddress(opengl_module, "wglShareLists");
        mimas_wglSwapBuffers = (PFN_wglSwapBuffers)SwapBuffers;

        HDC const prev_hdc = wglGetCurrentDC();
        HGLRC const prev_hglrc = wglGetCurrentContext();

        HGLRC const hglrc = wglCreateContext(hdc);
        if(!hglrc) {
            // TODO: Error
            return mimas_false;
        }

        if(!wglMakeCurrent(hdc, hglrc)) {
            wglMakeCurrent(prev_hdc, prev_hglrc);
            wglDeleteContext(hglrc);
            // TODO: Error
            return mimas_false;
        }

        mimas_wglGetExtensionsStringARB = (PFN_wglGetExtensionsStringARB)wglGetProcAddress("wglGetExtensionsStringARB");
        mimas_wglCreateContextAttribsARB = (PFN_wglCreateContextAttribsARB)wglGetProcAddress("wglCreateContextAttribsARB");
        mimas_wglGetPixelFormatAttribivARB = (PFN_wglGetPixelFormatAttribivARB)wglGetProcAddress("wglGetPixelFormatAttribivARB");
        mimas_wglGetPixelFormatAttribfvARB = (PFN_wglGetPixelFormatAttribfvARB)wglGetProcAddress("wglGetPixelFormatAttribfvARB");
        mimas_wglChoosePixelFormatARB = (PFN_wglChoosePixelFormatARB)wglGetProcAddress("wglChoosePixelFormatARB");
        mimas_wglSwapIntervalEXT = (PFN_wglSwapIntervalEXT)wglGetProcAddress("wglSwapIntervalEXT");
        mimas_wglGetSwapIntervalEXT = (PFN_wglGetSwapIntervalEXT)wglGetProcAddress("wglGetSwapIntervalEXT");

        wglMakeCurrent(prev_hdc, prev_hglrc);
        wglDeleteContext(hglrc);
        return mimas_true;
    } else {
        // TODO: Error
        return mimas_false;
    }
}

void mimas_unload_wgl(void) {
    mimas_wglGetProcAddress = NULL;
    mimas_wglGetExtensionsStringARB = NULL;
    mimas_wglCreateContextAttribsARB = NULL;
    mimas_wglGetPixelFormatAttribivARB = NULL;
    mimas_wglGetPixelFormatAttribfvARB = NULL;
    mimas_wglChoosePixelFormatARB = NULL;
    mimas_wglSwapIntervalEXT = NULL;
    mimas_wglGetSwapIntervalEXT = NULL;
    FreeLibrary(opengl_module);
}
