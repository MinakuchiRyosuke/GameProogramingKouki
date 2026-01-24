#pragma once

#include <DirectXMath.h>

class Camera final
{
public:
	//コンスタントバッファ用データ構造体
	struct ConstantBufferData {
		DirectX::XMMATRIX view_{};	//ビュー行列
		DirectX::XMMATRIX projection_{};	//射影行列
	};

public:
	Camera() = default;
	~Camera() = default;

public:
	//カメラを初期化
	void initialize() noexcept;

	//カメラを更新
	void update() noexcept;

	//カメラのビュー行列を取得する
	[[nodiscard]] DirectX::XMMATRIX XM_CALLCONV viewMatrix() const noexcept;

	//プロジェクション行列を取得
	[[nodiscard]] DirectX::XMMATRIX XM_CALLCONV projection() const noexcept;

private:
	DirectX::XMMATRIX view_{};	//ビュー行列
	DirectX::XMMATRIX projection_{};	//射影行列

	//カメラのパラメータ
	DirectX::XMFLOAT3 position_{};	//カメラの位置
	DirectX::XMFLOAT3 target_{};	//カメラの注視点
	DirectX::XMFLOAT3 up_{};	//カメラの上方向
};

