//入力処理を担当するクラス
#include "input.h"

//キー情報の取得
//key  キーの識別子
//return  入力していればtrue
[[nodiscard]] bool Input::getKey(uint16_t sKey) const noexcept {
	return (keyState_[sKey] & 0x80) != 0;
}

//キー情報の取得
//key  キーの識別子
//return  入力したらtrue
[[nodiscard]] bool Input::getTrigger(uint16_t sKey) const noexcept {
	return ((keyState_[sKey] & 0x80) != 0) && ((prevKeyState_[sKey] & 0x80) == 0);
}

//キー情報の更新
//pState  ハードから設定されたキー情報配列のポインタ
void Input::updateKeyState(void* pState) noexcept {
	memcpy_s(keyState_.data(), keyState_.size(), pState, keyState_.size());
}

//キー情報の更新
void Input::updatePrevKeyState() noexcept {
	memcpy_s(prevKeyState_.data(), prevKeyState_.size(), keyState_.data(), keyState_.size());
}

