#ifndef WINDOW_H
#define WINDOW_H

#include <windows.h>

class Window {
public:
    Window(HINSTANCE hInstance, int nCmdShow);
    ~Window();

    void Run();
    HWND GetHandle() const;

private:
    void Initialize();
    static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    HINSTANCE hInstance;
    HWND hWnd;
};

#endif // WINDOW_H