#pragma once

#ifndef WINDOW_MANAGER_H
#define WINDOW_MANAGER_H

#include <windows.h>

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);

class WindowManager{
private:
    LPCSTR CLASS_NAME = "TMSC Window";

    HWND m_hWnd;
    HINSTANCE m_hInstance;
    WNDCLASS m_wndClass;


public:
    WindowManager();
    ~WindowManager();

    bool ProcessMessage();

    static int testcolor;
};

#endif