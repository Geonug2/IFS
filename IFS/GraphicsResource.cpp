#include "GraphicsResource.h"

GraphicsResource::GraphicsResource(Microsoft::WRL::ComPtr<ID3D12Device> device)
    : m_device(device), m_width(0), m_height(0) {
}

GraphicsResource::~GraphicsResource() {
    // Automaatne vabastamine, kuna kasutame ComPtr
}

bool GraphicsResource::Initialize(UINT width, UINT height) {
    m_width = width;
    m_height = height;

    // Ressursi kirjeldus
    D3D12_RESOURCE_DESC resourceDesc = {};
    resourceDesc.MipLevels = 1;
    resourceDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    resourceDesc.Width = m_width;
    resourceDesc.Height = m_height;
    resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
    resourceDesc.SampleDesc.Count = 1;
    resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
    resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;

    // Looge heap properties
    D3D12_HEAP_PROPERTIES heapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

    // Ressursi loomine
    HRESULT hr = m_device->CreateCommittedResource(
        &heapProperties,
        D3D12_HEAP_FLAG_NONE,
        &resourceDesc,
        D3D12_RESOURCE_STATE_RENDER_TARGET,
        nullptr,
        IID_PPV_ARGS(&m_resource)
    );

    return SUCCEEDED(hr);
}