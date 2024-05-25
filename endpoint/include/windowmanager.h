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

    Display* m_display; // Display
    int m_src;  // Main screen
    
    Window m_rootWnd; // Root window
    std::unordered_map<Window, Window> m_clients; // Map of: window -> frame
    std::vector<Window> m_wnds; // List of windows to cycle with NextWindow
    std::vector<Window>::iterator m_currentWnd; // Current selected window from windows list

    bool m_eventloop; // Event loop - while based
    XEvent m_event; // Buffer for event

    unsigned int m_width; // Width of main screen
    unsigned int m_height; // Height of main screen

    /**
     * Opens x11 display.
     * Function required for constructor.
    */
    Display* OpenDisplay();

    /**
     * Frames window, maps keys handling and sets required variables
     * @param w window to frame
    */
    void Frame(Window);
    /**
     * Unframes window and removes from m_clients and m_wnds
     * @param w window to unframe
    */
    void Unframe(Window);
    /**
     * Closes window. Sents request to close if is supported, else kills window
     * @param w window to close
    */
    void Close(Window);

    /**
     * Events
    */

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

    /**
     * Atom const
    */

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
     * Switches to next window (raises next window to top)
    */
    void NextWindow();
    /**
     * Stops event loop
    */
    void StopEventLoop();
};

#endif