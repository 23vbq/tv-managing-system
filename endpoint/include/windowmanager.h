#ifndef WINDOW_MANAGER_H
#define WINDOW_MANAGER_H

#define WM_POSX 0
#define WM_POSY 0
/*#define WM_WIDTH 500
#define WM_HEIGHT 500*/
#define WM_BORDER 0

#include <X11/Xlib.h>
#include <unordered_map>
#include <stdio.h>
#include <cstdlib>
#include <syslog.h>
// #include <err.h>

class WindowManager{
private:
    static bool s_wm_detected; // Is other WM detected

    Display* m_display;
    int m_src;  // Main screen
    
    Window m_rootWnd;
    std::unordered_map<Window, Window> m_clients;
    Window m_wnd; // FIXME for testing probably won't be in use

    XEvent m_event; // Buffer for event

    unsigned int m_width; // Width of main screen
    unsigned int m_height; // Height of main screen
    // FIXME test
    unsigned char *t_image32;
    XImage *t_img;

    void Frame(Window);
    void Unframe(Window);

    void OnCreateNotify(const XCreateWindowEvent&);
    void OnConfigureRequest(const XConfigureRequestEvent&);
    void OnMapRequest(const XMapRequestEvent&);
    void OnUnmapNotify(const XUnmapEvent&);

    // Static functions

    /**
     * Handler for XErrorEvent - all
    */
    static int OnXError(Display*, XErrorEvent*);
    /**
     * Handler for XErrorEvent - detected other WM
    */
    static int OnWMDetected(Display*, XErrorEvent*);
public:
    WindowManager();
    ~WindowManager();

    void CreateWindow();
    /**
     * Start handler for WindowManager
    */
    void Run();
};

#endif