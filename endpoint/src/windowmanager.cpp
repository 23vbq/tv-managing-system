#include "windowmanager.h"
#include <X11/Xutil.h>

// Static members

bool WindowManager::s_wm_detected = false;
const unsigned long WindowManager::BG_COLOR = 0x0000FF;

// Constructor

WindowManager::WindowManager()
    : m_display(OpenDisplay()),
      m_src(DefaultScreen(m_display)),
      m_rootWnd(RootWindow(m_display, m_src)),
      WM_PROTOCOLS(XInternAtom(m_display, "WM_PROTOCOLS", false)),
      WM_DELETE_WINDOW(XInternAtom(m_display, "WM_DELETE_WINDOW", false))
{
    ptr_s_termination = nullptr;

    // Initialize width, height
    m_width = WidthOfScreen(ScreenOfDisplay(m_display, m_src));
    m_height = HeightOfScreen(ScreenOfDisplay(m_display, m_src));

    XGrabKey(
        m_display,
        XKeysymToKeycode(m_display, XK_Q),
        Mod1Mask,
        m_rootWnd,
        false,
        GrabModeAsync,
        GrabModeAsync
    );

    syslog(LOG_INFO, "Created [Display: %i, Src: %i, Root: %i]", m_display, m_src, m_rootWnd);
    syslog(LOG_INFO, "Determined resolution [W: %i, H: %i]", m_width, m_height);
}
WindowManager::~WindowManager(){
    auto clients = m_clients;
    for (auto it = clients.begin(); it != clients.end(); it++){
        XUnmapWindow(m_display, it->first);
        Unframe(it->first);
        Close(it->first);
    }
    XCloseDisplay(m_display);
}

Display* WindowManager::OpenDisplay(){
    Display* dpy = XOpenDisplay(NULL);
    if (dpy == NULL)
        throw "Cannot open display";
    return dpy;
}

// Main loop (Run) function

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
    // Get attributes of window
    XWindowAttributes x_wnd_attr;
    XGetWindowAttributes(m_display, w, &x_wnd_attr);
    
    // Create frame window
    const Window frame = XCreateSimpleWindow(
        m_display, m_rootWnd,
        x_wnd_attr.x, x_wnd_attr.y, x_wnd_attr.width, x_wnd_attr.height,
        WM_BORDER, 0, BG_COLOR
    );
    XSelectInput(m_display, frame, SubstructureRedirectMask | SubstructureNotifyMask);
    XAddToSaveSet(m_display, w);
    XReparentWindow(m_display, w, frame, 0, 0);
    XMapWindow(m_display, frame);
    m_clients[w] = frame;
    m_wnds.push_back(w);
    m_currentWnd = m_wnds.end() - 1;
    syslog(LOG_DEBUG, "Framed: %i", w);
    XGrabKey(
        m_display,
        XKeysymToKeycode(m_display, XK_Q),
        Mod1Mask,
        w,
        false,
        GrabModeAsync,
        GrabModeAsync
    );
    XGrabKey(
        m_display,
        XKeysymToKeycode(m_display, XK_F4),
        Mod1Mask,
        w,
        false,
        GrabModeAsync,
        GrabModeAsync
    );
    XGrabKey(
        m_display,
        XKeysymToKeycode(m_display, XK_Tab),
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
    // FIXME need to be tested
    // Switch to next window
    if (w == *m_currentWnd)
        NextWindow();
    // Unframing
    const Window frame = m_clients[w];
    XReparentWindow(m_display, w, m_rootWnd, 0, 0);
    XRemoveFromSaveSet(m_display, w);
    XUnmapWindow(m_display, frame);
    XDestroyWindow(m_display, frame);
    // Remove from lists / maps
    m_clients.erase(w);
    size_t n = m_wnds.size();
    for (size_t i = 0; i < n; i++){
        if (m_wnds[i] == w){
            m_wnds.erase(m_wnds.begin() + i);
            break;
        }
    }
}
void WindowManager::Close(Window w){
    Atom* supported_prot;
    int n_supported_prot;
    if (XGetWMProtocols(m_display, w, &supported_prot, &n_supported_prot) &&
        std::find(supported_prot, supported_prot + n_supported_prot, WM_DELETE_WINDOW) != supported_prot + n_supported_prot){
            syslog(LOG_INFO, "Deleting window %i", w);
            // Constructing message
            XEvent msg;
            memset(&msg, 0, sizeof(msg));
            msg.xclient.type = ClientMessage;
            msg.xclient.message_type = WM_PROTOCOLS;
            msg.xclient.window = w;
            msg.xclient.format = 32;
            msg.xclient.data.l[0] = WM_DELETE_WINDOW;
            XSendEvent(m_display, w, false, 0, &msg);
    } else {
        syslog(LOG_INFO, "Killing window %i", w);
        XKillClient(m_display, w);
    }
}

void WindowManager::OnCreateNotify(const XCreateWindowEvent &e) {}
void WindowManager::OnConfigureRequest(const XConfigureRequestEvent &e){
    XWindowChanges changes;
    changes.x = WM_POSX;
    changes.y = WM_POSY;
    changes.width = m_width;
    changes.height = m_height;
    changes.border_width = WM_BORDER;
    changes.sibling = e.above;
    changes.stack_mode = e.detail;
    XConfigureWindow(m_display, e.window, e.value_mask, &changes);
}
void WindowManager::OnMapRequest(const XMapRequestEvent &e){
    Frame(e.window);
    XMapWindow(m_display, e.window);
    XSetInputFocus(m_display, e.window, RevertToPointerRoot, CurrentTime);
}
void WindowManager::OnUnmapNotify(const XUnmapEvent &e){
    if (e.event == m_rootWnd){
        syslog(LOG_INFO, "Ignoring unmapping of root");
        return;
    }
    Unframe(e.window);
}
void WindowManager::OnKeyPress(const XKeyEvent &e){
    // Alt handler
    if (e.state == Mod1Mask){
        // Exit WM on ALT + Q
        if (e.keycode == XKeysymToKeycode(m_display, XK_Q)){
            m_eventloop = false;
            if (ptr_s_termination != nullptr)
                *ptr_s_termination = true;
            return;
        }
        // Close window on ALT + F4
        if (e.keycode == XKeysymToKeycode(m_display, XK_F4)){
            Close(e.window);
            return;
        }
        // FIXME For testing
        if (e.keycode == XKeysymToKeycode(m_display, XK_Tab)){
            NextWindow();
            return;
        }
        return;
    }  
}

// Public functions

void WindowManager::SetPtrSTermination(bool *termination){
    ptr_s_termination = termination;
}

void WindowManager::NextWindow(){
    if (++m_currentWnd == m_wnds.end())
        m_currentWnd = m_wnds.begin();
    /*for (auto it = m_wnds.begin(); it != m_wnds.end(); it++){

    }*/
    XRaiseWindow(m_display, m_clients[*m_currentWnd]);
    XSetInputFocus(m_display, *m_currentWnd, RevertToPointerRoot, CurrentTime);
    syslog(LOG_INFO, "Raised %i", *m_currentWnd);
    XSync(m_display, 0);
}
void WindowManager::StopEventLoop(){
    m_eventloop = false;
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