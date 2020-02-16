#include <win/wgl.h>

#include <wingdi.h>

static HMODULE opengl_module = NULL;

typedef void* (APIENTRY* PWGLGETPROCADDRESS)(char const*);
static PWGLGETPROCADDRESS mimas_wglGetProcAddress = NULL;
#define wglGetProcAddress mimas_wglGetProcAddress

HGLRC (APIENTRY* mimas_wglCreateContext)(HDC hdc);
BOOL (APIENTRY* mimas_wglDeleteContext)(HGLRC hglrc);
HDC (APIENTRY* mimas_wglGetCurrentDC)();
HGLRC (APIENTRY* mimas_wglGetCurrentContext)();
BOOL (APIENTRY* mimas_wglMakeCurrent)(HDC hdc, HGLRC hglrc);
BOOL (APIENTRY* mimas_wglShareLists)(HGLRC hglrc1, HGLRC hglrc2);
void (APIENTRY* mimas_wglSwapBuffers)(HDC hdc);

char const*(APIENTRY* mimas_wglGetExtensionsStringARB)(HDC hdc) = NULL;
HGLRC (APIENTRY* mimas_wglCreateContextAttribsARB)(HDC hDC, HGLRC hShareContext, int const* attribList) = NULL;
BOOL (WINAPI* mimas_wglGetPixelFormatAttribivARB)(HDC hdc, int iPixelFormat, int iLayerPlane, UINT nAttributes, int const* piAttributes, int* piValues) = NULL;
BOOL (WINAPI* mimas_wglGetPixelFormatAttribfvARB)(HDC hdc, int iPixelFormat, int iLayerPlane, UINT nAttributes, int const* piAttributes, FLOAT* pfValues) = NULL;
BOOL (WINAPI* mimas_wglChoosePixelFormatARB)(HDC hdc, int const* piAttribIList, FLOAT const* pfAttribFList, UINT nMaxFormats, int* piFormats, UINT* nNumFormats) = NULL;
BOOL (WINAPI* mimas_wglSwapIntervalEXT)(int interval) = NULL;
int (WINAPI* mimas_wglGetSwapIntervalEXT)(void) = NULL;

mimas_bool mimas_load_wgl(HDC const hdc) {
    opengl_module = LoadLibraryW(L"opengl32.dll");
    if(opengl_module) {
        mimas_wglGetProcAddress = (PWGLGETPROCADDRESS)GetProcAddress(opengl_module, "wglGetProcAddress");
        mimas_wglCreateContext = (PWGLCREATECONTEXT)GetProcAddress(opengl_module, "wglCreateContext");
        mimas_wglDeleteContext = (PWGLDELETECONTEXT)GetProcAddress(opengl_module, "wglDeleteContext");
        mimas_wglGetCurrentDC = (PWGLGETCURRENTDC)GetProcAddress(opengl_module, "wglGetCurrentDC");
        mimas_wglGetCurrentContext = (PWGLGETCURRENTCONTEXT)GetProcAddress(opengl_module, "wglGetCurrentContext");
        mimas_wglMakeCurrent = (PWGLMAKECURRENT)GetProcAddress(opengl_module, "wglMakeCurrent");
        mimas_wglShareLists = (PWGLSHARELISTS)GetProcAddress(opengl_module, "wglShareLists");
        mimas_wglSwapBuffers = (PWGLSWAPBUFFERS)SwapBuffers;

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

        mimas_wglGetExtensionsStringARB = (PWGLGETEXTENSIONSSTRINGARB)wglGetProcAddress("wglGetExtensionsStringARB");
        mimas_wglCreateContextAttribsARB = (PWGLCREATECONTEXTATTRIBSARB)wglGetProcAddress("wglCreateContextAttribsARB");
        mimas_wglGetPixelFormatAttribivARB = (PWGLGETPIXELFORMATATTRIBIVARB)wglGetProcAddress("wglGetPixelFormatAttribivARB");
        mimas_wglGetPixelFormatAttribfvARB = (PWGLGETPIXELFORMATATTRIBFVARB)wglGetProcAddress("wglGetPixelFormatAttribfvARB");
        mimas_wglChoosePixelFormatARB = (PWGLCHOOSEPIXELFORMATARB)wglGetProcAddress("wglChoosePixelFormatARB");
        mimas_wglSwapIntervalEXT = (PWGLSWAPINTERVALEXT)wglGetProcAddress("wglSwapIntervalEXT");
        mimas_wglGetSwapIntervalEXT = (PWGLGETSWAPINTERVALEXT)wglGetProcAddress("wglGetSwapIntervalEXT");

        wglMakeCurrent(prev_hdc, prev_hglrc);
        wglDeleteContext(hglrc);
        return mimas_true;
    } else {
        // TODO: Error
        return mimas_false;
    }
}

void mimas_unload_wgl() {
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
