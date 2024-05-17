#include "windowmanager.h"

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

    // Main loop
    for (;;){
        XNextEvent(m_display, &m_event);
        
        switch (m_event.type)
        {
        case CreateNotify:
            OnCreateNotify(m_event.xcreatewindow);
            break;
        case ConfigureRequest:
            OnConfigureRequest(m_event.xconfigurerequest);
            break;
        case MapRequest:
            OnMapRequest(m_event.xmaprequest);
            break;
        default:
            break;
        }
        /*if (XNextEvent(m_display, &m_event) == 0)
            break;*/
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

// Static private functions

int WindowManager::OnWMDetected(Display *display, XErrorEvent *e){
    if (e->error_code == BadAccess)
        WindowManager::s_wm_detected = true;
    return 0;
}
int WindowManager::OnXError(Display *display, XErrorEvent *e){
    return 0;
}