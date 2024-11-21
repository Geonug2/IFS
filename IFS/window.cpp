#include "window.h"
#include "VertexShader.h"

VertexShader m_vertexShader;

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

bool Window::InitializeDirectX() {
    // Enable debug layer
    #if defined(_DEBUG)
    {
        Microsoft::WRL::ComPtr<ID3D12Debug> debugController;
        if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
            debugController->EnableDebugLayer();
        }
    }
    #endif

    // Create DXGI factory
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

    // Initialize Pixel Shader
    if (!m_pixelShader.Initialize(m_device)) {
        return false;
    }

    // Initialize Vertex Shader
    if (!m_vertexShader.Initialize(m_device)) {
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
        // Taustavärv must
        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_BACKGROUND + 1)); // Must värv
        EndPaint(hWnd, &ps);
    }
    return 0;
    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}