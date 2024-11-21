#include "VertexShader.h"
#include <iostream>
#include <d3dcompiler.h>
#include <fstream>
#include <vector>

VertexShader::VertexShader() {
}

VertexShader::~VertexShader() {
}

bool VertexShader::Initialize(Microsoft::WRL::ComPtr<ID3D12Device> device) {
    // HLSL faili laadimine
    std::ifstream shaderFile("VertexShader.hlsl");
    if (!shaderFile.is_open()) {
        std::cerr << "Failed to open VertexShader.hlsl" << std::endl;
        return false;
    }

    std::string shaderCode((std::istreambuf_iterator<char>(shaderFile)),
        std::istreambuf_iterator<char>());
    shaderFile.close();

    Microsoft::WRL::ComPtr<ID3DBlob> errorBlob;

    HRESULT hr = D3DCompile(
        shaderCode.c_str(),
        shaderCode.size(),
        nullptr,
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
        return false;
    }

    return true;
}