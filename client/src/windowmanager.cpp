#include "windowmanager.h"

int WindowManager::testcolor = 1;

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

    ShowWindow(m_hWnd, SW_SHOW);
}

WindowManager::~WindowManager(){
    UnregisterClass(CLASS_NAME, m_hInstance);
}

// Private functions

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    case WM_PAINT:
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW + WindowManager::testcolor++));
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