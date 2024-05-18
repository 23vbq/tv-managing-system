#include "windowmanager.h"
#include <X11/Xutil.h>

// Static members
bool WindowManager::s_wm_detected = false;

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
    auto clients = m_clients;
    for (auto it = clients.begin(); it != clients.end(); it++){
        XUnmapWindow(m_display, it->first);
        Unframe(it->first);
        XDestroyWindow(m_display, it->first);
        // TODO bezpieczne zamknięcie okienka
    }
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
    // Initialization
    s_wm_detected = false;
    XSetErrorHandler(&WindowManager::OnWMDetected);
    XSelectInput(m_display,
        m_rootWnd,
        SubstructureRedirectMask | SubstructureNotifyMask
    );
    XSync(m_display, false);
    if (s_wm_detected){
        return;
    }
    XSetErrorHandler(&WindowManager::OnXError);

    m_eventloop = true;
    // Main loop
    while (m_eventloop){
        syslog(LOG_DEBUG, "LOOP");
        XNextEvent(m_display, &m_event);
        
        switch (m_event.type)
        {
        case CreateNotify:
            syslog(LOG_DEBUG, "CreateNotify");
            OnCreateNotify(m_event.xcreatewindow);
            break;
        case ConfigureRequest:
            syslog(LOG_DEBUG, "ConfigureRequest");
            OnConfigureRequest(m_event.xconfigurerequest);
            break;
        case MapRequest:
            syslog(LOG_DEBUG, "MapRequest");
            OnMapRequest(m_event.xmaprequest);
            break;
        case UnmapNotify:
            syslog(LOG_DEBUG, "UnmapNotify");
            OnUnmapNotify(m_event.xunmap);
        case KeyPress:
            syslog(LOG_DEBUG, "KeyPress");
            OnKeyPress(m_event.xkey);
        default:
            syslog(LOG_DEBUG, "Ignore");
            break;
        }
    }
}

// Private functions

void WindowManager::Frame(Window w){
    const unsigned int BORDER_WIDTH = 3;
    const unsigned long BORDER_COLOR = 0xFF0000;
    const unsigned long BG_COLOR = 0x0000FF;
    // Get attributes of window
    XWindowAttributes x_wnd_attr;
    XGetWindowAttributes(m_display, w, &x_wnd_attr);
    
    // Create frame window
    const Window frame = XCreateSimpleWindow(
        m_display, m_rootWnd,
        x_wnd_attr.x, x_wnd_attr.y, x_wnd_attr.width, x_wnd_attr.height,
        BORDER_WIDTH, BORDER_COLOR, BG_COLOR
    );
    XSelectInput(m_display, frame, SubstructureRedirectMask | SubstructureNotifyMask);
    XAddToSaveSet(m_display, frame);
    XReparentWindow(m_display, w, frame, 0, 0);
    XMapWindow(m_display, frame);
    m_clients[w] = frame;
    XGrabKey(
        m_display,
        XKeysymToKeycode(m_display, XK_Q),
        Mod1Mask,
        w,
        false,
        GrabModeAsync,
        GrabModeAsync
    );
}
void WindowManager::Unframe(Window w){
    if (!m_clients.count(w)){
        syslog(LOG_WARNING, "Window to unframe not found");
        return;
    }
    const Window frame = m_clients[w];
    XReparentWindow(m_display, w, m_rootWnd, 0, 0);
    XRemoveFromSaveSet(m_display, frame);
    XUnmapWindow(m_display, frame);
    m_clients.erase(w);
}

void WindowManager::OnCreateNotify(const XCreateWindowEvent &e) {}
void WindowManager::OnConfigureRequest(const XConfigureRequestEvent &e){
    XWindowChanges changes;
    changes.x = e.x;
    changes.y = e.y;
    changes.width = e.width;
    changes.height = e.height;
    changes.border_width = e.border_width;
    changes.sibling = e.above;
    changes.stack_mode = e.detail;
    XConfigureWindow(m_display, e.window, e.value_mask, &changes);
}
void WindowManager::OnMapRequest(const XMapRequestEvent &e){
    Frame(e.window);
    XMapWindow(m_display, e.window);
}
void WindowManager::OnUnmapNotify(const XUnmapEvent &e){
    if (e.event == m_rootWnd){
        syslog(LOG_INFO, "Ignoring unmapping of root");
        return;
    }
    Unframe(e.window);
}
void WindowManager::OnKeyPress(const XKeyEvent &e){
    // Exit if ALT+Q
    if (e.state == Mod1Mask &&
        e.keycode == XKeysymToKeycode(m_display, XK_Q)){
            m_eventloop = false;
        }
}

// Static private functions

int WindowManager::OnWMDetected(Display *display, XErrorEvent *e){
    if (e->error_code == BadAccess)
        WindowManager::s_wm_detected = true;
    return 0;
}
int WindowManager::OnXError(Display *display, XErrorEvent *e){
    return 0;
}