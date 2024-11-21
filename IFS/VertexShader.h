#pragma once
#include <d3d12.h>
#include <wrl/client.h>
#include <d3dcompiler.h>

class VertexShader {
public:
    VertexShader();
    ~VertexShader();

    bool Initialize(Microsoft::WRL::ComPtr<ID3D12Device> device);
    Microsoft::WRL::ComPtr<ID3DBlob> GetShaderBlob() { return m_vertexShaderBlob; }

private:
    Microsoft::WRL::ComPtr<ID3DBlob> m_vertexShaderBlob;
};