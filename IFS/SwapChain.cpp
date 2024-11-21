#include "SwapChain.h"

SwapChain::SwapChain(Microsoft::WRL::ComPtr<ID3D12Device> device, HWND hwnd, UINT width, UINT height)
    : m_device(device), m_hwnd(hwnd), m_width(width), m_height(height) {
}

SwapChain::~SwapChain() {
    if (m_swapChain) {
        m_swapChain->Release();
    }
}

bool SwapChain::Initialize() {
    DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
    swapChainDesc.Width = m_width;
    swapChainDesc.Height = m_height;
    swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.Stereo = FALSE;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.BufferCount = 2;
    swapChainDesc.Scaling = DXGI_SCALING_NONE;
    swapChainDesc.Flags = 0;

    Microsoft::WRL::ComPtr<IDXGIFactory4> factory;
    CreateDXGIFactory1(IID_PPV_ARGS(&factory));

    HRESULT hr = factory->CreateSwapChainForHwnd(
        m_device.Get(),
        m_hwnd,
        &swapChainDesc,
        nullptr,
        nullptr,
        &m_swapChain // Muuda see IDXGISwapChain1-ks
    );

    return SUCCEEDED(hr);
}

void SwapChain::Present() {
    if (m_swapChain) {
        Microsoft::WRL::ComPtr<IDXGISwapChain3> swapChain3;
        m_swapChain.As(&swapChain3); // Konverteerime IDXGISwapChain1 IDXGISwapChain3-ks
        swapChain3->Present(1, 0); // 1 for vsync, 0 for no flags
    }
}