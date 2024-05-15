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
    // FIXME test
    t_image32 = (unsigned char*)calloc(m_width*m_height, sizeof(char) * 4);
    unsigned char *p = t_image32;
    double hw = m_height / 0xFF;
    double ww = m_width / 0xFF;
    for (int i = 0; i < m_height; i++){
        for (int j = 0; j < m_width; j++){
            *p++ = i / hw;
            *p++ = 0;
            *p++ = j / ww;
            *p++ = 0;
        }
    }
    t_img = XCreateImage(m_display, DefaultVisual(m_display, m_src), DefaultDepth(m_display, m_src), ZPixmap, 0, (char*)t_image32, m_width, m_height, 32, 0);
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
    // FIXME test
    XPutImage(m_display, m_wnd, DefaultGC(m_display, m_src), t_img, 0, 0, 0, 0, m_width, m_height);
}
void WindowManager::Run(){
    for (;;){
        if (XNextEvent(m_display, &m_event) == 0)
            break;
    }
}