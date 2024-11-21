#include "window.h"
#include "GraphicsResource.h"

Window::Window(HINSTANCE hInstance, int nCmdShow) : hInstance(hInstance), m_swapChain(nullptr, nullptr, 1360, 768) {
    const wchar_t CLASS_NAME[] = L"InfinitySpaceWindowClass";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    hWnd = CreateWindowExW(
        0,
        CLASS_NAME,
        L"Infinity Space",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 1360, 768,
        NULL, NULL, hInstance, NULL
    );

    ShowWindow(hWnd, nCmdShow);
}

bool Window::InitializeDirectX() {

    Microsoft::WRL::ComPtr<IDXGIFactory4> factory;
    if (FAILED(CreateDXGIFactory1(IID_PPV_ARGS(&factory)))) {
        return false;
    }

    // Create device
    if (FAILED(D3D12CreateDevice(
        nullptr,
        D3D_FEATURE_LEVEL_12_0,
        IID_PPV_ARGS(&m_device)
    ))) {
        return false;
    }

    // Create command queue
    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
    queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    if (FAILED(m_device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_commandQueue)))) {
        return false;
    }

    GraphicsResource graphicsResource(m_device);
    if (!graphicsResource.Initialize(1360, 768)) {
        return false; // Viga ressursi loomisel
    }

    // Tehke midagi ressursiga, nt salvestage see liikuvasse muutujasse
    m_graphicsResource = graphicsResource.GetResource();

    // Initialize Pixel Shader
    if (!m_pixelShader.Initialize(m_device)) {
        return false;
    }

    // Initialize Vertex Shader
    if (!m_vertexShader.Initialize(m_device)) {
        return false;
    }

    if (!m_swapChain.Initialize()) {
        return false;
    }

    return true;
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
        // Taustav�rv must
        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_BACKGROUND + 1)); // Must v�rv
        EndPaint(hWnd, &ps);
    }
    return 0;
    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}