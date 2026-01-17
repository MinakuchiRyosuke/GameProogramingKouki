//入力処理を担当するクラス
#pragma once
#include<Windows.h>
#include<cstdint>
#include<array>

class Input final
{
public:
	//インスタンスの取得
	//インスタンスの参照
	static Input& instance() noexcept {
		static Input instance;
		return instance;
	}

	//キー情報の取得
	//key  キーの識別子
	//return  入力していればtrue
	[[nodiscard]] bool getKey(uint16_t sKey) const noexcept;

	//キー情報の取得
	//key  キーの識別子
	//return  入力したらtrue
	[[nodiscard]] bool getTrigger(uint16_t skey) const noexcept;

	//キー情報の更新
	//pState  ハードから設定されたキー情報配列のポインタ
	void updateKeyState(void* pState) noexcept;

	//キー情報の更新
	void updatePrevKeyState() noexcept;

private:
	//シングルトンパターンにするため、コンストラクタとデストラクタをprivateにする
	Input() = default;
	~Input() = default;

	//コピーとムーブの禁止
	Input(const Input& r) = delete;
	Input& operator= (const Input& r) = delete;
	Input(Input&& r)  = delete;
	Input& operator =(Input&& r) = delete;

private:
	std::array<byte, 256> keyState_{};		//ハードから設定されたキー情報配列
	std::array<byte, 256> prevKeyState_{};	//ハードから設定されたキー情報配列
};

