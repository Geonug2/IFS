#pragma once
#include <d3d12.h>
#include <wrl/client.h>
#include <d3dcompiler.h>
#include <string>

class VertexShader {
public:
    VertexShader();
    ~VertexShader();

    bool Initialize(Microsoft::WRL::ComPtr<ID3D12Device> device);
    bool LoadShaderFromFile(Microsoft::WRL::ComPtr<ID3D12Device> device, const std::wstring& filename);
    Microsoft::WRL::ComPtr<ID3DBlob> GetShaderBlob() { return m_vertexShaderBlob; }

private:
    Microsoft::WRL::ComPtr<ID3DBlob> m_vertexShaderBlob;
};