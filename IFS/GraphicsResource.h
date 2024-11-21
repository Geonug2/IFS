#ifndef GRAPHICSRESOURCE_H
#define GRAPHICSRESOURCE_H

#include <d3d12.h>
#include <d3dx12.h> // Lisage see rida
#include <wrl/client.h>

class GraphicsResource {
public:
    GraphicsResource(Microsoft::WRL::ComPtr<ID3D12Device> device);
    ~GraphicsResource();

    bool Initialize(UINT width, UINT height);
    ID3D12Resource* GetResource() const { return m_resource.Get(); }

private:
    Microsoft::WRL::ComPtr<ID3D12Device> m_device;
    Microsoft::WRL::ComPtr<ID3D12Resource> m_resource;
    UINT m_width;
    UINT m_height;
};

#endif // GRAPHICSRESOURCE_H