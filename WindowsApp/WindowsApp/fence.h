//フェンス（CPUとGPUの動機）制御クラス
#pragma once
#include"Dx12.h"
//---------------------------------------------------------------------------------
/**
 * @brief	フェンス（CPUとGPUの動機）制御クラス
 */
class Fence final {
public :
	//コンストラクタ
	Fence() = default;
	//デストラクタ
	~Fence();
	//フェンスを作成
	[[nodiscard]] bool create(const Dx12& device)noexcept;
	//---------------------------------------------------------------------------------
	/**
	 * @brief	同期待ちを行う
	 * @param fenceValue	フェンス値
	 */
	void wait(UINT64 fenceValue)const noexcept;
	//---------------------------------------------------------------------------------
	/**
	 * @brief	フェンスを取得する
	 */
	[[nodiscard]] ID3D12Fence* get() const noexcept;

private:
	ID3D12Fence* fence_{};			///フェンス
	HANDLE		 waitGpuEvent_{};	///GPUとCPU同期用のイベントハンドル
};