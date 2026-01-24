#pragma once

#include <DirectXMath.h>

class Object final
{
public:
	//コンスタントバッファ用データ構造体
	struct ConstantBufferData {
		DirectX::XMMATRIX world_{};	//ワールド行列
		DirectX::XMFLOAT4 color_{};	//カラー
	};

public:
	Object() = default;
	~Object() = default;
	void update() noexcept;

	//ワールド行列の取得
	[[nodiscard]] DirectX::XMMATRIX world() const noexcept;

	//カラーの取得
	[[nodiscard]] DirectX::XMFLOAT4 color() const noexcept;

private:
	DirectX::XMMATRIX world_ = DirectX::XMMatrixIdentity();		//ワールド行列
	DirectX::XMFLOAT4 color_ = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);	//カラー

	float move_{};	//移動用変数
};

