#include "window.h"

#include "Dx12.h"
#include "CommandAllocator.h"
#include "command_list.h"
#include "descriptor_heap.h"
#include "render_target.h"
#include "fence.h"

#include <cassert>

class Application final {
public:
	Application() = default;
	~Application() = default;

	//アプリケーションの初期化
	[[nodiscard]] bool initialize(HINSTANCE instance)noexcept {
		//ウィンドウの作成
		if (S_OK != windowInstance_.create(instance, 1280, 720, "MyApp")) {
			assert(false && "ウィンドウの作成に失敗しました");
			return false;
		}
		//DXGIの作成
		if (!dx12Instance_.setDisplayAdapter()) {
			assert(false && "DXGIの作成に失敗しました");
			return false;
		}
		//デバイスの作成
		if (!dx12Instance_.createDevice(dx12Instance_)) {
			assert(false && "デバイスの作成に失敗しました");
			return false;
		}
		//コマンドキューの生成
		if (!dx12Instance_.createCommandQueue(dx12Instance_)) {
			assert(false && "コマンドキューの作成に失敗しました");
			return false;
		}
		//スワップチェインの生成
		if (!dx12Instance_.createSwapChain(dx12Instance_, windowInstance_)) {
			assert(false && "ディスクリプタヒープの作成に失敗しました");
			return false;
		}
		//ディスクリプタヒープの生成
		if (!descriptorHeapInstance_.create(dx12Instance_, D3D12_DESCRIPTOR_HEAP_TYPE_RTV, dx12Instance_.getDesc().BufferCount)) {
			assert(false && "ディスクリプタヒープの生成に失敗しました");
			return false;
		}

		//レンダーターゲットの生成
		if (!renderTargetInstance_.createBackBuffer(dx12Instance_, descriptorHeapInstance_)) {
			assert(false && "レンダーターゲットの作成に失敗しました");
			return false;
		}
		//コマンドアロケータの生成
		if (!commandAllocatorInstance_[0].create(dx12Instance_, D3D12_COMMAND_LIST_TYPE_DIRECT)) {
			assert(false && "コマンドアロケータの作成に失敗しました");
			return false;
		}
		if (!commandAllocatorInstance_[1].create(dx12Instance_, D3D12_COMMAND_LIST_TYPE_DIRECT) ){
			assert(false && "コマンドアロケータの作成に失敗しました");
			return false;
		}
		//コマンドリストの生成
		if (!commandListInstance_.create(dx12Instance_, commandAllocatorInstance_[0])) {
			assert(false && "コマンドリストの作成に失敗しました");
			return false;
		}
		//フェンスの生成
		if (!fenceInstance_.create(dx12Instance_)) {
			assert(false && "フェンスの作成に失敗しました");
			return false;
		}

		return true;
	}

	//アプリケーションループ
	void loop() noexcept {
		while (windowInstance_.messageLoop()) {
			//現在のバックバッファインデックスを取得
		}
	}

private:
	Window				windowInstance_{};
	Dx12				dx12Instance_{};
	DescriptorHeap		descriptorHeapInstance_{};
	RenderTarget		renderTargetInstance_{};
	CommandAllocator	commandAllocatorInstance_[2]{};
	CommandList			commandListInstance_{};

	Fence	fenceInstance_{};
	UINT64	frameFenceValue_[2]{};
	UINT64	nextFenceValue_ = 1;
};

//エントリー関数
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	//アプリケーションクラスのインスタンスを生成
	Application app;

	if (!app.initialize(hInstance)) {
		assert(false && "アプリケーションの初期化に失敗しました");
	}

	//アプリケーションループ開始
	app.loop();

	return 0;
}







































//#include <Windows.h>
//
//LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
//{
//    switch (uMsg)
//    {
//    case WM_DESTROY:    //×ボタンが押されたとき
//        PostQuitMessage(0);     //「アプリを終了します」とWindowsに通知
//        return 0;
//    case WM_PAINT:      //ウィンドウの再描画が必要な時
//        //画面を更新する処理をここに書く
//        return 0;
//    case WM_KEYDOWN:    //キーが押されたとき
//        //キー入力したときの処理
//        return 0;
//    }
//    //自分で処理しないメッセージはWindowsに任せる
//    return DefWindowProc(hwnd, uMsg, wParam, lParam);
//}
//
//int WINAPI WinMain(
//    HINSTANCE hInstance,      // アプリケーションの識別番号
//    HINSTANCE hPrevInstance,  // 基本使わなくていい
//    LPSTR lpCmdLine,          // コマンドライン引数（起動時のオプション）
//    int nCmdShow              // ウィンドウの表示方法（最大化、最小化など）
//
//
//)
//{
//    // ここにメインの処理を書く
//    // 1. ウィンドウクラス登録
//    WNDCLASS wc{};
//    wc.lpfnWndProc = WindowProc;            //ウィンドウプロシージャを指定
//    wc.hInstance = hInstance;               //アプリケーションインスタス
//    wc.lpszClassName = "GameWindow";        //ウィンドウクラス名 
//    wc.hCursor = LoadCursor(NULL, IDC_ARROW);   //マウスカーソル
//    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);     //背景色
//
//    RegisterClass(&wc);     //Windowsに登録
//
//    // 2. ウィンドウ作成
//    HWND hwnd = CreateWindow(
//        "GameWidow",        //ウィンドウクラス名
//        "My Game",          //ウィンドウタイトル
//        WS_OVERLAPPEDWINDOW,    //ウィンドウスタイル
//        CW_USEDEFAULT, CW_USEDEFAULT,   //位置（自動）
//        800, 600,               //サイズ（幅、高さ）
//        NULL, NULL,             //親ウィンドウ、　メニュー
//        hInstance,              //インスタンス
//        NULL                    //追加データ
//    );
//
//    ShowWindow(hwnd, nCmdShow); //ウィンドウを表示
//
//
//    bool nextFrame = true;
//    while (nextFrame)
//    {
//        // 3. メッセージループ
//        MSG msg{};      //メッセージを格納する構造体
//        while (PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE))     //メッセージが来るまで待機
//        {
//            if (msg.message == WM_QUIT)
//            {
//                nextFrame = false;
//            }
//            TranslateMessage(&msg);     //キーボードメッセージを使いやすい形に変換
//            DispatchMessage(&msg);      //適切なウィンドウプロシージャに送信 
//        }
//    }
//    
//
//    return 0;
//    
//}

