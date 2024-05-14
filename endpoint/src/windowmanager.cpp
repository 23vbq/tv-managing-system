#include "windowmanager.h"

// Constructor

WindowManager::WindowManager(){
    // Open display
    m_display = XOpenDisplay(NULL);
    if (m_display == NULL)
        throw "Cannot open display";
    // Get default screen and root window
    m_src = DefaultScreen(m_display);
    m_rootWnd = RootWindow(m_display, m_src);
    // Initialize width, height
    m_width = WidthOfScreen(ScreenOfDisplay(m_display, m_src));
    m_height = HeightOfScreen(ScreenOfDisplay(m_display, m_src));
}
WindowManager::~WindowManager(){
    XUnmapWindow(m_display, m_wnd);
    XDestroyWindow(m_display, m_wnd);
    XCloseDisplay(m_display);
}

// Public functions

void WindowManager::CreateWindow(){
    m_wnd = XCreateSimpleWindow(m_display, m_rootWnd,
        WM_POSX, WM_POSY, m_width, m_height, WM_BORDER,
        BlackPixel(m_display, m_src), WhitePixel(m_display, m_src)
    );
    XMapWindow(m_display, m_wnd);
}
void WindowManager::Run(){
    for (;;){
        if (XNextEvent(m_display, &m_event) == 0)
            break;
    }
}