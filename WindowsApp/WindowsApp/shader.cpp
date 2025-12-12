#include "shader.h"
#include <cassert>
#include <string>

#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

Shader::~Shader() {
	//頂点シェーダの解放
	if (vertexShader_) {
		vertexShader_->Release();
		vertexShader_ = nullptr;
	}
	//ピクセルシェーダの解放
	if (pixelShader_) {
		pixelShader_->Release();
		pixelShader_ = nullptr;
	}
}

//シェーダを作成する
[[nodiscard]] bool Shader::create(const Dx12& dx12) noexcept {
	// シェーダを読込、コンパイルして生成する

	// シェーダファイルのパス
	const std::string filePath = "shader.hlsl";
	const std::wstring temp = std::wstring(filePath.begin(), filePath.end());

	// シェーダのコンパイルエラーなどが分かる様にする
	ID3DBlob* error{};

	auto res = D3DCompileFromFile(temp.data(), nullptr, nullptr, "vs", "vs_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &vertexShader_, &error);
	if (FAILED(res)) {
		char* p = static_cast<char*>(error->GetBufferPointer());
		assert(false && "頂点シェーダのコンパイルに失敗しました");
	}
	res = D3DCompileFromFile(temp.data(), nullptr, nullptr, "ps", "ps_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &pixelShader_, &error);
	if (FAILED(res)) {
		char* p = static_cast<char*>(error->GetBufferPointer());
		assert(false && "ピクセルシェーダのコンパイルに失敗しました");
	}

	if (error) {
		error->Release();
	}

	return true;
}

//頂点データを取得する
[[nodiscard]] ID3DBlob* Shader::vertexShader() const noexcept {
	if (!vertexShader_) {
		assert(false && "頂点シェーダが未作成です");
	}

	return vertexShader_;
}

//ピクセルシェーダを取得する
[[nodiscard]] ID3DBlob* Shader::pixelShader() const noexcept {
	if (!pixelShader_) {
		assert(false && "ピクセルシェーダが未作成です");
	}

	return pixelShader_;
}