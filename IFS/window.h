#ifndef WINDOW_H
#define WINDOW_H

#include <windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl/client.h>
#include "PixelShader.h"
#include "VertexShader.h"
#include "SwapChain.h"

class Window {
public:
    Window(HINSTANCE hInstance, int nCmdShow);
    ~Window();
    void Show();
    HWND GetHandle() const;
    bool InitializeDirectX();

private:
    HWND hWnd;
    HINSTANCE hInstance;

    // DirectX 12 members
    Microsoft::WRL::ComPtr<ID3D12Device> m_device;
    Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_commandQueue;
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_rtvHeap;

    PixelShader m_pixelShader;
    VertexShader m_vertexShader;

    // Muuda see rida, et viidata SwapChain klassile
    SwapChain m_swapChain;

    static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};

#endif // WINDOW_H