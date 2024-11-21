#pragma once
#include <d3d12.h>
#include <wrl/client.h>
#include <d3dcompiler.h>
#include <string>

class PixelShader {
public:
    PixelShader();
    ~PixelShader();

    bool Initialize(Microsoft::WRL::ComPtr<ID3D12Device> device);
    Microsoft::WRL::ComPtr<ID3DBlob> GetShaderBlob() { return m_pixelShaderBlob; }

private:
    Microsoft::WRL::ComPtr<ID3DBlob> m_pixelShaderBlob;
};