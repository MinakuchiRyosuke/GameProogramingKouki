#include "camera.h"
#include <cmath>

namespace {
	//定数
	constexpr float eyeMoveSpeed_ = 0.06f;	//カメラの移動速度
	constexpr float destTargetToView_ = -5.0f;	//注視点からカメラまでの距離
}

//カメラを初期化
void Camera::initialize() noexcept {
	//カメラ位置の設定
	position_ = DirectX::XMFLOAT3(0.0f, 0.0f, destTargetToView_);
	//カメラの注視点を設定
	target_ = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	//カメラの上方向を設定
	up_ = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);

	//プロジェクション行列の設定
	projection_ = DirectX::XMMatrixPerspectiveFovLH(
		DirectX::XM_PIDIV4,	//視野角４５度
		1280.0f / 720.0f,	//アスペクト比
		0.1f,				//ニアグリップ
		100.0f				//ファーグリップ
	);
}

//カメラを更新
void Camera::update() noexcept {
	static float angle = 0.0f;
	angle += eyeMoveSpeed_;
	position_.x = destTargetToView_ * std::sinf(angle);
	position_.z = destTargetToView_ * std::cosf(angle);

	// ビュー行列の計算
	view_ = DirectX::XMMatrixLookAtLH(
		DirectX::XMLoadFloat3(&position_),
		DirectX::XMLoadFloat3(&target_),
		DirectX::XMLoadFloat3(&up_));
}

//カメラのビュー行列を取得
[[nodiscard]] DirectX::XMMATRIX XM_CALLCONV Camera::viewMatrix() const noexcept {
	return view_;
}

//プロジェクション行列を取得
[[nodiscard]] DirectX::XMMATRIX XM_CALLCONV Camera::projection() const noexcept {
	return projection_;
}