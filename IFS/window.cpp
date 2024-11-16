#include "window.h"
#include <stdexcept>

Window::Window(HINSTANCE hInstance, int nCmdShow) : hInstance(hInstance) {
    Initialize();
    ShowWindow(hWnd, nCmdShow);
}

Window::~Window() {
    DestroyWindow(hWnd);
}

void Window::Initialize() {
    const wchar_t CLASS_NAME[] = L"Sample Window Class";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    // Saame ekraani suuruse
    RECT screenRect;
    screenRect.left = 0;
    screenRect.top = 0;
    screenRect.right = GetSystemMetrics(SM_CXSCREEN);
    screenRect.bottom = GetSystemMetrics(SM_CYSCREEN);

    // Loome akna, mis katab kogu ekraani
    hWnd = CreateWindowEx(
        0,
        CLASS_NAME,
        L"DirectX 12 Window", // Muudetud L"DirectX 12 Window"
        WS_POPUP, // Kasutame WS_POPUP, et eemaldada akna piirded
        screenRect.left, screenRect.top,
        screenRect.right - screenRect.left,
        screenRect.bottom - screenRect.top,
        nullptr,
        nullptr,
        hInstance,
        nullptr
    );

    if (hWnd == nullptr) {
        throw std::runtime_error("Failed to create window");
    }

    // Täisekraanil akna jaoks
    SetWindowLong(hWnd, GWL_STYLE, WS_POPUP);
    SetWindowPos(hWnd, HWND_TOP, 0, 0, screenRect.right - screenRect.left, screenRect.bottom - screenRect.top, SWP_NOZORDER | SWP_FRAMECHANGED);
}

void Window::Run() {
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

HWND Window::GetHandle() const {
    return hWnd;
}

LRESULT CALLBACK Window::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}