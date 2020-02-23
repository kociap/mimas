#include "init.h"
#include <dlfcn.h>

PFN_XOpenDisplay mimas_XOpenDisplay;
PFN_XDefaultScreen mimas_XDefaultScreen;
PFN_XScreenOfDisplay mimas_XScreenOfDisplay;
PFN_XCreateSimpleWindow mimas_XCreateSimpleWindow;
PFN_XRootWindow mimas_XRootWindow;
PFN_XBlackPixel mimas_XBlackPixel;
PFN_XWhitePixel mimas_XWhitePixel;
PFN_XSelectInput mimas_XSelectInput;
PFN_XMapWindow mimas_XMapWindow;
PFN_XDefaultGC mimas_XDefaultGC;
PFN_XCloseDisplay mimas_XCloseDisplay;
PFN_XStoreName mimas_XStoreName;
PFN_XSetIconName mimas_XSetIconName;

mimas_bool mimas_load_x11() {
    void* x11_module = dlopen("libx11.so.6", RTLD_LOCAL | RTLD_LAZY);

    if (!x11_module) {
        return mimas_false;
    }

    mimas_XOpenDisplay = dlsym(x11_module, "XOpenDisplay");
    mimas_XDefaultScreen = dlsym(x11_module, "XDefaultScreen");
    mimas_XScreenOfDisplay = dlsym(x11_module, "XScreenOfDisplay");
    mimas_XCreateSimpleWindow = dlsym(x11_module, "XCreateSimpleWindow");
    mimas_XRootWindow = dlsym(x11_module, "XRootWindow");
    mimas_XBlackPixel = dlsym(x11_module, "XBlackPixel");
    mimas_XWhitePixel = dlsym(x11_module, "XWhitePixel");
    mimas_XSelectInput = dlsym(x11_module, "XSelectInput");
    mimas_XMapWindow = dlsym(x11_module, "XMapWindow");
    mimas_XDefaultGC = dlsym(x11_module, "XDefaultGC");
    mimas_XCloseDisplay = dlsym(x11_module, "XCloseDisplay");
    mimas_XStoreName = dlsym(x11_module, "XStoreName");
    mimas_XSetIconName = dlsym(x11_module, "XSetIconName");

    dlclose(x11_module);

    return mimas_true;
}