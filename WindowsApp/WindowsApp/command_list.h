#pragma once
#include"Dx12.h"
#include"CommandAllocator.h"

//---------------------------------------------------------------------------------
/**
 * @brief	コマンドリスト制御クラス
 */
class CommandList final {
public:
	//コンストラクタ
	CommandList() = default;
	//デストラクタ
	~CommandList();

	//---------------------------------------------------------------------------------
	/**
	 * @brief	コマンドリスト作成
	 * @param	device	デバイスクラスのインスタンス
	 * @param	command	コマンドアロケータクラスのインスタンス
	 * @return	生成の成否
	 */
	[[nodiscard]] bool create(const Dx12& dx12, const CommandAllocator& commandAllocator)noexcept;
	//---------------------------------------------------------------------------------
	/**
	* @brief	コマンドリストのリセット
	* @param	commandAllocator	コマンドアロケータクラスのインスタンス
	*/
	void reset(const CommandAllocator& commandAllocator)noexcept;
	//---------------------------------------------------------------------------------
	/**
	* @brief	コマンドリストを取得する
	* @return	コマンドリストのポインタ
	*/
	[[nodiscard]] ID3D12GraphicsCommandList* get()const noexcept;

private:
	ID3D12GraphicsCommandList* commandList_{};		//コマンドリスト
};