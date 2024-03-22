#pragma once

#ifndef WINDOW_MANAGER_H
#define WINDOW_MANAGER_H

#include <windows.h>

class WindowManager{
private:
    HNINSTANCE m_hInstance;
    WNDCLASS m_wc;

    LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
public:
    WindowManager();
};

#endif