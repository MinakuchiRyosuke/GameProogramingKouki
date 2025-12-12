#pragma once

#include"Dx12.h"
class Shader final
{
public:
	Shader() = default;
	~Shader();

	//シェーダを作成する
	[[nodiscard]] bool create(const Dx12& dx12) noexcept;

	//頂点シェーダを取得する
	[[nodiscard]] ID3DBlob* vertexShader() const noexcept;

	//ピクセルシェーダを取得する
	[[nodiscard]] ID3DBlob* pixelShader() const noexcept;

private:
	ID3DBlob* vertexShader_{};	///頂点シェーダ
	ID3DBlob* pixelShader_{};	///ピクセルシェーダ
};

