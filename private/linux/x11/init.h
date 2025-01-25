#ifndef MIMAS_INIT_H
#define MIMAS_INIT_H

#include <X11/Xlib.h>
#include <mimas/mimas.h>

typedef Display* (*PFN_XOpenDisplay)(char const*);
typedef int (*PFN_XDefaultScreen)(Display*);
typedef Screen* (*PFN_XScreenOfDisplay)(Display*, int);
typedef Window (*PFN_XCreateSimpleWindow)(Display*, Window, int, int,
                                          unsigned int, unsigned int,
                                          unsigned int, unsigned long,
                                          unsigned long);
typedef Window (*PFN_XRootWindow)(Display*, int);
typedef unsigned long (*PFN_XBlackPixel)(Display*, int);
typedef unsigned long (*PFN_XWhitePixel)(Display*, int);
typedef int (*PFN_XSelectInput)(Display*, Window, long);
typedef int (*PFN_XMapWindow)(Display*, Window);
typedef GC (*PFN_XDefaultGC)(Display*, int);
typedef int (*PFN_XCloseDisplay)(Display*);
typedef int (*PFN_XStoreName)(Display*, Window, char const*);
typedef int (*PFN_XSetIconName)(Display*, Window, char const*);

extern PFN_XOpenDisplay mimas_XOpenDisplay;
extern PFN_XDefaultScreen mimas_XDefaultScreen;
extern PFN_XScreenOfDisplay mimas_XScreenOfDisplay;
extern PFN_XCreateSimpleWindow mimas_XCreateSimpleWindow;
extern PFN_XRootWindow mimas_XRootWindow;
extern PFN_XBlackPixel mimas_XBlackPixel;
extern PFN_XWhitePixel mimas_XWhitePixel;
extern PFN_XSelectInput mimas_XSelectInput;
extern PFN_XMapWindow mimas_XMapWindow;
extern PFN_XDefaultGC mimas_XDefaultGC;
extern PFN_XCloseDisplay mimas_XCloseDisplay;
extern PFN_XStoreName mimas_XStoreName;
extern PFN_XSetIconName mimas_XSetIconName;

mimas_bool mimas_load_x11(void);

#endif //MIMAS_INIT_H
