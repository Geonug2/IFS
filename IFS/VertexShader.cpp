#include "VertexShader.h"
#include <iostream>

VertexShader::VertexShader() {
}

VertexShader::~VertexShader() {
}

bool VertexShader::Initialize(Microsoft::WRL::ComPtr<ID3D12Device> device) {
    // Basic vertex shader code
    const char* shaderCode = R"(
        struct VSInput {
            float3 position : POSITION;
            float4 color : COLOR;
        };

        struct VSOutput {
            float4 position : SV_POSITION;
            float4 color : COLOR;
        };

        VSOutput main(VSInput input) {
            VSOutput output;
            output.position = float4(input.position, 1.0f);
            output.color = input.color;
            return output;
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