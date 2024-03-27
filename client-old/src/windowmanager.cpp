#include "windowmanager.h"

HWND WindowManager::s_hWnd = NULL;

// Constructors

WindowManager::WindowManager()
    : m_hInstance(GetModuleHandle(nullptr)){

    m_wndClass = {};
    m_wndClass.lpfnWndProc = WindowProc;
    m_wndClass.hInstance = m_hInstance;
    m_wndClass.lpszClassName = CLASS_NAME;
    m_wndClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
    m_wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);

    RegisterClass(&m_wndClass);

    DWORD style = WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;

    s_hWnd = CreateWindowEx(
        0,
        CLASS_NAME,
        "TMSC Test",
        style,
        CW_USEDEFAULT, CW_USEDEFAULT, W_WIDTH + 5, W_HEIGHT + 29,
        NULL,
        NULL,
        m_hInstance,
        NULL
    );

    int iHalfWindowWidth = (W_WIDTH - W_PADDING * 2) / 2 - 5;
    HWND leftWindow = CreateWindowEx(
        0,
        "STATIC",
        "Test1",
        WS_VISIBLE | WS_CHILD | WS_BORDER,
        W_PADDING, W_PADDING, iHalfWindowWidth, W_HEIGHT - W_PADDING * 2,
        s_hWnd,
        NULL, NULL, NULL
    );
    
    // SetBkColor
    // m_childs.push_back(leftWindow);

    HWND rightWindow = CreateWindowEx(
        0,
        "STATIC",
        "Test2",
        WS_VISIBLE | WS_CHILD | WS_BORDER,
        W_WIDTH / 2 + 5, W_PADDING, iHalfWindowWidth, W_HEIGHT - W_PADDING * 2,
        s_hWnd,
        NULL, NULL, NULL
    );

    /*HWND text1 = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        TEXT("Edit"),
        TEXT("Test"),
        WS_VISIBLE | WS_CHILD,
        10, 10, 100, 40,
        m_hWnd,
        NULL, NULL, NULL
    );*/

    HWND button1 = CreateWindowEx(
        0,
        "Button",
        "Fajny buton",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD,
        10, 50, 200, 20,
        s_hWnd,
        (HMENU) 1, (HINSTANCE)GetWindowLongPtr(s_hWnd, GWLP_HINSTANCE), NULL
    );
    /*HWND button2 = CreateWindowEx(
        0,
        "Button",
        "Lepszy buton",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD,
        10, 80, 200, 20,
        m_hWnd,
        (HMENU) 2, (HINSTANCE)GetWindowLongPtr(m_hWnd, GWLP_HINSTANCE), NULL
    );*/
    /*HDC hdc = GetDC(leftWindow);
    SetBkColor(hdc, RGB(0, 255, 0));
    InvalidateRect(leftWindow, NULL, TRUE);
    ReleaseDC(leftWindow, hdc);*/

    ShowWindow(s_hWnd, SW_SHOW);
    UpdateWindow(s_hWnd);
}

WindowManager::~WindowManager(){
    UnregisterClass(CLASS_NAME, m_hInstance);
}

// Private functions
COLORREF WindowManager::bg = RGB(255, 255, 255);
LRESULT CALLBACK WindowManager::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
    HDC hdc;
    switch (uMsg)
    {
    // case WM_CREATE:
    //     HWND textfield1 = CreateWindow(
    //         "STATIC",
    //         "Testowy napis",
    //         WS_VISIBLE | WS_CHILD,
    //         20, 20, 100, 20,
    //         &,
    //         NULL, NULL, NULL
    //     );
    //     break;
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case 1:
        {
            ::MessageBeep(MB_ICONERROR);
            ::MessageBoxA(hwnd, "Wystapil oczekiwany blad!", "Dziala ale bez polskich znakow", MB_ICONERROR);
            bg = RGB(0, 255, 0);
            InvalidateRect(hwnd, NULL, TRUE);
            HWND test = CreateWindowEx(
                0,
                "Edit",
                "",
                WS_CHILD | WS_VISIBLE | WS_BORDER,
                200, 200, 400, 400,
                s_hWnd,
                NULL, NULL, NULL
            );
            break;
        }
        case 2:
            ::MessageBeep(MB_OK);
            ::MessageBoxA(hwnd, "A ten blad jest jeszcze lepszy i bardziej oczekiwany!", "Lepszy", MB_ICONWARNING);
            break;
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    case WM_PAINT:
        PAINTSTRUCT ps;
        hdc = BeginPaint(hwnd, &ps);
        //FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW + 1));
        EndPaint(hwnd, &ps);
        return 0;
    case WM_ERASEBKGND:
        hdc = (HDC)wParam;
        RECT rect;
        GetClientRect(hwnd, &rect);
        HBRUSH brush = CreateSolidBrush(bg); // RGB(0, 255, 0) to kolor zielony
        FillRect(hdc, &rect, brush);
        DeleteObject(brush);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

// Public functions

bool WindowManager::ProcessMessage(){
    MSG msg = {};
    while (PeekMessage(&msg, nullptr, 0u, 0u, PM_REMOVE)){
        if (msg.message == WM_QUIT)
            return false;
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return true;
}