#ifndef WINDOW_MANAGER_H
#define WINDOW_MANAGER_H

#define WM_POSX 500
#define WM_POSY 500
#define WM_WIDTH 500
#define WM_HEIGHT 500
#define WM_BORDER 15

#include <X11/Xlib.h>
#include <stdio.h>
#include <err.h>

class WindowManager{
private:
    Display* dpy;
    int src;
    
    Window root;
    Window win;
public:
    WindowManager();
    ~WindowManager();

    void Create();
    void Run();
};

#endif