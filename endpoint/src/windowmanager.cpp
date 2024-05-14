#include "windowmanager.h"

// Constructor

WindowManager::WindowManager(){
    m_display = XOpenDisplay(NULL);
    if (m_display == NULL)
        throw "Cannot open display";
    m_src = DefaultScreen(m_display);
    m_rootWnd = RootWindow(m_display, m_src);
}
WindowManager::~WindowManager(){

}

// Public functions

void WindowManager::CreateWindow(){
    m_wnd = XCreateSimpleWindow(m_display, m_rootWnd,
        WM_POSX, WM_POSY, WM_WIDTH, WM_HEIGHT, WM_BORDER,
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