#include "window.h"

namespace {
LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}
}

[[nodiscard]] HRESULT Window::create(HINSTANCE instance, int width, int height, std::string_view name)noexcept {
	//ウィンドウの定義
	WNDCLASS wc{};
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = instance;
	wc.lpszClassName = name.data();
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);

	//ウィンドウクラスの登録
	RegisterClass(&wc);


	//ウィンドウの作成
	handle_ = CreateWindow(wc.lpszClassName, wc.lpszClassName,
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height,
		nullptr, nullptr, instance, nullptr);
	if (!handle_) {
		return E_FAIL;
	}

	//ウィンドウの表示
	ShowWindow(handle_, SW_SHOW);

	//ウィンドウを更新
	UpdateWindow(handle_);

	//ウィンドウのサイズを保存
	width_ = width;
	height_ = height;

	//成功を返す
	return S_OK;
}

//メッセージループ
[[nodiscard]] bool Window::messageLoop() const noexcept {
	MSG msg{};
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
		if (msg.message == WM_QUIT) {
			return false;
		}
		//メッセージ処理
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return true;
}

//ウィンドウハンドルを取得する
[[nodiscard]] HWND Window::handle() const noexcept {
	return handle_;
}

//ウィンドウのサイズを取得する
[[nodiscard]] std::pair<int, int> Window::size() const noexcept {
	return { width_, height_ };
}