//コマンドアロケータ制御クラス
#pragma once
#include"Dx12.h"
//---------------------------------------------------------------------------------
/**
 * @brief	コマンドアロケータ制御クラス
 */
class CommandAllocator final {
public:
	//コンストラクタ
	CommandAllocator() = default;
	//デストラクタ
	~CommandAllocator();

	//---------------------------------------------------------------------------------
	/**
	 * @brief	コマンドアロケータ作成
	 * @param	device	デバイスクラスのインスタンス
	 * @param	type	コマンドリストのタイプ
	 */
	[[nodiscard]] bool create(const Dx12& dx12, const D3D12_COMMAND_LIST_TYPE type)noexcept;
	//---------------------------------------------------------------------------------
	/**
	 * @brief	コマンドアロケータをリセットする
	 */
	void reset()noexcept;
	//---------------------------------------------------------------------------------
	/**
	 * @brief	コマンドアロケータを取得する
	 * @return	コマンドアロケータのポインタ
	 */
	[[nodiscard]] ID3D12CommandAllocator* get()const noexcept;
	//---------------------------------------------------------------------------------
	/**
	 * @brief	コマンドリストのタイプを取得する
	 * @return	コマンドリストのタイプ
	 */
	[[nodiscard]] D3D12_COMMAND_LIST_TYPE getType() const noexcept;

private:
	ID3D12CommandAllocator* commandAllocator_{};	///コマンドアロケータ
	D3D12_COMMAND_LIST_TYPE type_{};				///コマンドリストのタイプ
};