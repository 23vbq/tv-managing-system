#pragma once

#ifndef WINDOW_MANAGER_H
#define WINDOW_MANAGER_H

#define W_WIDTH 1280
#define W_HEIGHT 720
#define W_PADDING 10

#include <windows.h>
#include <wingdi.h>
#include <iostream>
#include <vector>

class WindowManager{
private:
    LPCSTR CLASS_NAME = "TMSC Window";

    static HWND s_hWnd;
    HINSTANCE m_hInstance;
    WNDCLASS m_wndClass;

    // std::vector<HWND> m_childs;
    static COLORREF bg;

    static LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
public:
    WindowManager();
    ~WindowManager();

    bool ProcessMessage();
};

#endif