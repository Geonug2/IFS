#include "PixelShader.h"
#include <iostream>

PixelShader::PixelShader() {
}

PixelShader::~PixelShader() {
}

bool PixelShader::Initialize(Microsoft::WRL::ComPtr<ID3D12Device> device) {
    // Basic pixel shader code
    const char* shaderCode = R"(
        struct PSInput {
            float4 position : SV_POSITION;
            float4 color : COLOR;
        };

        float4 main(PSInput input) : SV_TARGET {
            return float4(1.0f, 0.0f, 0.0f, 1.0f); // Red color
        }
    )";

    Microsoft::WRL::ComPtr<ID3DBlob> errorBlob;

    HRESULT hr = D3DCompile(
        shaderCode,
        strlen(shaderCode),
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