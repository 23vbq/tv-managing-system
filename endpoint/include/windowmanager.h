#ifndef WINDOW_MANAGER_H
#define WINDOW_MANAGER_H

#define WM_POSX 0
#define WM_POSY 0
/*#define WM_WIDTH 500
#define WM_HEIGHT 500*/
#define WM_BORDER 0

#include <X11/Xlib.h>
#include <stdio.h>
#include <cstdlib>
// #include <err.h>

class WindowManager{
private:
    static bool s_wm_detected;

    Display* m_display;
    int m_src;
    
    Window m_rootWnd;
    Window m_wnd;

    XEvent m_event;

    unsigned int m_width;
    unsigned int m_height;
    // FIXME test
    unsigned char *t_image32;
    XImage *t_img;

    static int OnXError(Display*, XErrorEvent*);
    static int OnWMDetected(Display*, XErrorEvent*);
public:
    WindowManager();
    ~WindowManager();

    void CreateWindow();
    void Run();
};

#endif