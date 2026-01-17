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

private:
	Input() = default;
	~Input();
};

