#ifndef WINDOW_MANAGER_H
#define WINDOW_MANAGER_H

#define WM_POSX 0
#define WM_POSY 0
/*#define WM_WIDTH 500
#define WM_HEIGHT 500*/
#define WM_BORDER 0

#include <X11/Xlib.h>
#include <unordered_map>
#include <vector>
#include <stdio.h>
#include <cstdlib>
#include <algorithm>
#include <cstring>
#include <syslog.h>
// #include <err.h>

class WindowManager{
private:
    static bool s_wm_detected; // Is other WM detected

    Display* m_display;
    int m_src;  // Main screen
    
    Window m_rootWnd;
    std::unordered_map<Window, Window> m_clients;
    std::vector<Window> m_wnds;
    std::vector<Window>::iterator m_currentWnd;

    bool m_eventloop;
    XEvent m_event; // Buffer for event

    unsigned int m_width; // Width of main screen
    unsigned int m_height; // Height of main screen

    // Construct

    Display* OpenDisplay();

    void Frame(Window);
    void Unframe(Window);
    void Close(Window);

    void OnCreateNotify(const XCreateWindowEvent&);
    void OnConfigureRequest(const XConfigureRequestEvent&);
    void OnMapRequest(const XMapRequestEvent&);
    void OnUnmapNotify(const XUnmapEvent&);
    void OnKeyPress(const XKeyEvent&);

    // Static functions

    /**
     * Handler for XErrorEvent - all
    */
    static int OnXError(Display*, XErrorEvent*);
    /**
     * Handler for XErrorEvent - detected other WM
    */
    static int OnWMDetected(Display*, XErrorEvent*);

    // Atom const

    const Atom WM_PROTOCOLS;
    const Atom WM_DELETE_WINDOW;
public:
    WindowManager();
    ~WindowManager();

    /**
     * Start handler for WindowManager. Main event loop.
    */
    void Run();
    /**
     * Switches to next window (raises next window)
    */
    void NextWindow();
};

#endif