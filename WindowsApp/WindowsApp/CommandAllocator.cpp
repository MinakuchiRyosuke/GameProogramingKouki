//コマンドアロケータ制御クラス
#include"commandallocator.h"
#include<cassert>

//デストラクタ
CommandAllocator::~CommandAllocator() {
	//コマンドアロケータの解放
	if (commandAllocator_) {
		commandAllocator_->Release();
		commandAllocator_ = nullptr;
	}
}
//---------------------------------------------------------------------------------
/**
 * @brief	コマンドキューの生成
 * @param	device	デバイスクラスのインスタンス
 * @return	成功すれば true
 */
[[nodiscard]] bool CommandAllocator::create(const Dx12& dx12, const D3D12_COMMAND_LIST_TYPE type)noexcept {
	//コマンドリストのタイプを設定
	type_ = type;
	//コマンドアロケータの生成
	const auto hr = dx12.device->CreateCommandAllocator(type_, IID_PPV_ARGS(&commandAllocator_));
	if (FAILED(hr)) {
		assert(false && "コマンドアロケータの作成に失敗しました");
		return false;
	}
	return true;
}
//---------------------------------------------------------------------------------
/**
 * @brief	コマンドアロケータをリセットする
 */
void CommandAllocator::reset()noexcept {
	if (!commandAllocator_) {
		assert(false && "コマンドアロケータが未作成です");
	}
	commandAllocator_->Reset();
}
//---------------------------------------------------------------------------------
/**
 * @brief	コマンドアロケータを取得する
 * @return	コマンドアロケータのポインタ
 */
[[nodiscard]] ID3D12CommandAllocator* CommandAllocator::get() const noexcept {
	if (!commandAllocator_) {
		assert(false && "コマンドアロケータが未作成です");
		return nullptr;
	}
	return commandAllocator_;
}
//---------------------------------------------------------------------------------
/**
 * @brief	コマンドリストのタイプを取得する
 * @return	コマンドリストのタイプ
 */
[[nodiascard]] D3D12_COMMAND_LIST_TYPE CommandAllocator::getType() const noexcept {
	if (!commandAllocator_) {
		assert(false && "コマンドリストのタイプが未設定です");
	}
	return type_;
}