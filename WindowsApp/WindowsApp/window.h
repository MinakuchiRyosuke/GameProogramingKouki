#pragma once
#include<Windows.h>
#include<string>
class Window final
{
public:
	Window() = default;
	~Window() = default;

	//ウィンドウの生成
	[[nodiscard]] HRESULT create(HINSTANCE instance, int width, int height, std::string_view name) noexcept;

	//メッセージループ
	[[nodiscard]] bool messageLoop() const noexcept;

	//ウィンドウハンドルを取得する
	[[nodiscard]] HWND handle() const noexcept;

	//ウィンドウのサイズを取得する
	[[nodiscard]] std::pair<int, int> size() const noexcept;

private:
	HWND handle_{};
	int width_{};
	int height_{};
};


