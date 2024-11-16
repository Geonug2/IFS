#ifndef WINDOW_H
#define WINDOW_H

#include <windows.h>

class Window {
public:
    Window(HINSTANCE hInstance, int nCmdShow);
    ~Window();
    void Show();
    HWND GetHandle() const;

private:
    HWND hWnd;
    HINSTANCE hInstance;
    static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};

#endif // WINDOW_H