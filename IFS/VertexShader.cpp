#include "VertexShader.h"
#include <iostream>
#include <d3dcompiler.h>
#include <fstream>

VertexShader::VertexShader() {
}

VertexShader::~VertexShader() {
}

bool VertexShader::Initialize(Microsoft::WRL::ComPtr<ID3D12Device> device) {
    // Siin võiksime kasutada LoadShaderFromFile meetodit, et laadida shader failist
    return LoadShaderFromFile(device, L"VertexShader.hlsl");
}

bool VertexShader::LoadShaderFromFile(Microsoft::WRL::ComPtr<ID3D12Device> device, const std::wstring& filename) {
    Microsoft::WRL::ComPtr<ID3DBlob> errorBlob;

    HRESULT hr = D3DCompileFromFile(
        filename.c_str(),
        nullptr,
        nullptr,
        "main",
        "vs_5_1",  // DirectX 12 shader model
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
        0,
        &m_vertexShaderBlob,
        &errorBlob
    );

    if (FAILED(hr)) {
        if (errorBlob) {
            std::string errorMessage = static_cast<char*>(errorBlob->GetBufferPointer());
            OutputDebugStringA(errorMessage.c_str());
            std::cerr << "Vertex Shader Compilation Failed: " << errorMessage << std::endl;
        }
        // Konverteeri wstring UTF-8 stringiks
        std::wcerr << L"Shader loading failed for file: " << filename << std::endl;
        return false;
    }

    std::wcout << L"Shader loaded successfully from file: " << filename << std::endl;
    return true;
}