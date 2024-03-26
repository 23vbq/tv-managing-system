#include "windowmanager.h"

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

    m_hWnd = CreateWindowEx(
        0,
        CLASS_NAME,
        "TMSC Test",
        style,
        CW_USEDEFAULT, CW_USEDEFAULT, 450, 200,
        NULL,
        NULL,
        m_hInstance,
        NULL
    );

    HWND text1 = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        TEXT("Edit"),
        TEXT("Test"),
        WS_VISIBLE | WS_CHILD,
        10, 10, 100, 40,
        m_hWnd,
        NULL, NULL, NULL
    );

    HWND button1 = CreateWindowEx(
        0,
        "Button",
        "Fajny buton",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD,
        10, 50, 200, 20,
        m_hWnd,
        (HMENU) 1, (HINSTANCE)GetWindowLongPtr(m_hWnd, GWLP_HINSTANCE), NULL
    );
    HWND button2 = CreateWindowEx(
        0,
        "Button",
        "Lepszy buton",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD,
        10, 80, 200, 20,
        m_hWnd,
        (HMENU) 2, (HINSTANCE)GetWindowLongPtr(m_hWnd, GWLP_HINSTANCE), NULL
    );

    ShowWindow(m_hWnd, SW_SHOW);
    UpdateWindow(m_hWnd);
}

WindowManager::~WindowManager(){
    UnregisterClass(CLASS_NAME, m_hInstance);
}

// Private functions

LRESULT CALLBACK WindowManager::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
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
            ::MessageBeep(MB_ICONERROR);
            ::MessageBoxA(hwnd, "Wystapil oczekiwany blad!", "Dziala ale bez polskich znakow", MB_ICONERROR);
            break;
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
        HDC hdc = BeginPaint(hwnd, &ps);
        FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW + 1));
        EndPaint(hwnd, &ps);
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