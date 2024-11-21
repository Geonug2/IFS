#ifndef SWAPCHAIN_H
#define SWAPCHAIN_H

#include <d3d12.h>
#include <wrl/client.h>
#include <dxgi1_6.h>

class SwapChain {
public:
    SwapChain(Microsoft::WRL::ComPtr<ID3D12Device> device, HWND hwnd, UINT width, UINT height);
    ~SwapChain();

    bool Initialize();
    void Present();

private:
    Microsoft::WRL::ComPtr<IDXGISwapChain1> m_swapChain;
    Microsoft::WRL::ComPtr<ID3D12Device> m_device;
    HWND m_hwnd;
    UINT m_width;
    UINT m_height;
};

#endif // SWAPCHAIN_H