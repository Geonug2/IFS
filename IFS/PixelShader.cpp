#include "PixelShader.h"
#include <iostream>
#include <d3dcompiler.h>
#include <fstream>
#include <sstream>

PixelShader::PixelShader() {
}

PixelShader::~PixelShader() {
}

bool PixelShader::Initialize(Microsoft::WRL::ComPtr<ID3D12Device> device) {
    // Looge HLSL failist shaderi kood
    std::ifstream shaderFile("PixelShader.hlsl");
    if (!shaderFile.is_open()) {
        std::cerr << "Failed to open PixelShader.hlsl" << std::endl;
        return false;
    }

    std::stringstream buffer;
    buffer << shaderFile.rdbuf();
    std::string shaderCode = buffer.str();
    shaderFile.close();

    Microsoft::WRL::ComPtr<ID3DBlob> errorBlob;

    HRESULT hr = D3DCompile(
        shaderCode.c_str(),
        shaderCode.size(),
        nullptr,
        nullptr,
        nullptr,
        "main",
        "ps_5_1",  // DirectX 12 shader model
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
        0,
        &m_pixelShaderBlob,
        &errorBlob
    );

    if (FAILED(hr)) {
        if (errorBlob) {
            std::string errorMessage = static_cast<char*>(errorBlob->GetBufferPointer());
            OutputDebugStringA(errorMessage.c_str());
            std::cerr << "Pixel Shader Compilation Failed: " << errorMessage << std::endl;
        }
        return false;
    }

    return true;
}