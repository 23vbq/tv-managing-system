#include "windowmanager.h"

// Constructors

WindowManager::WindowManager()
    : m_hInstance(GetModuleHandle(nullptr)){
    const wchar_t CLASS_NAME[] = L"TMSC Window";

    m_wc = {};
    m_wc.lpfnWndProc = WindowProc;
    m_wc.hInstance = m_hInstance;
    m_wc.lpszClassName = CLASS_NAME;

    RegisterClass(&m_wc);
}

// Private functions

LRESULT CALLBACK WindowManager::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
    switch (uMsg)
    {
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
    return DefWindowProc(hwnd, uMsg, wParam, lParam)
}