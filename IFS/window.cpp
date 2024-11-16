#include "window.h"

Window::Window(HINSTANCE hInstance, int nCmdShow) : hInstance(hInstance) {
    const wchar_t CLASS_NAME[] = L"InfinitySpaceWindowClass";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    hWnd = CreateWindowExW(
        0,
        CLASS_NAME,
        L"Infinity Space",  // Muudame ka akna pealkirja lahti kirjutatud string'iks
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 1360, 768,
        NULL, NULL, hInstance, NULL
    );

    ShowWindow(hWnd, nCmdShow);
}

Window::~Window() {
    DestroyWindow(hWnd);
}

void Window::Show() {
    ShowWindow(hWnd, SW_SHOW);
}

HWND Window::GetHandle() const {
    return hWnd;
}

LRESULT CALLBACK Window::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // Taustavärv must
        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_BACKGROUND + 1)); // Must värv
        EndPaint(hWnd, &ps);
    }
    return 0;
    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}