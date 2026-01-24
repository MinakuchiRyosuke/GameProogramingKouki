#include "window.h"
#include "input.h"

#include "Dx12.h"
#include "CommandAllocator.h"
#include "command_list.h"
#include "descriptor_heap.h"
#include "render_target.h"
#include "fence.h"
#include "root_signature.h"
#include "shader.h"
#include "pipline_state_object.h"
#include "constant_buffer.h"

#include "triangle_polygon.h"
#include "square_polygon.h"

#include "camera.h"
#include "object.h"

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
		//四角形ポリゴンの生成
		if (!squarePolygonInstance_.create(dx12Instance_)) {
			assert(false && "四角形ポリゴンの作成に失敗しました");
			return false;
		}
		//三角形ポリゴンの生成
		if (!trianglePolygonInstance_.create(dx12Instance_)) {
			assert(false && "三角形ポリゴンの作成に失敗しました");
			return false;
		}
		// ルートシグネチャの生成
		if (!rootSignatureInstance_.create(dx12Instance_)) {
			assert(false && "ルートシグネチャの作成に失敗しました");
			return false;
		}
		// シェーダーの生成
		if (!shaderInstance_.create(dx12Instance_)) {
			assert(false && "シェーダーの作成に失敗しました");
			return false;
		}
		// パイプラインステートオブジェクトの生成
		if (!piplineStateObjectInstance_.create(dx12Instance_, shaderInstance_, rootSignatureInstance_)) {
			assert(false && "パイプラインステートオブジェクトの作成に失敗しました");
			return false;
		}
		// カメラの生成
		cameraInstance_.initialize();

		// 定数バッファ用ディスクリプタヒープの生成
		// 定数バッファ用は CBV_SRV_UAV タイプで2つ分確保
		if (!constantBufferDescriptorHeapInstance_.create(dx12Instance_, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 2, true)) {
			assert(false && "定数バッファ用ディスクリプタヒープの作成に失敗しました");
			return false;
		}

		// カメラ用コンスタントバッファの生成
		// カメラ用はディスクリプタヒープの先頭に作成
		if (!cameraConstantBufferInstance_.create(dx12Instance_, constantBufferDescriptorHeapInstance_, sizeof(Camera::ConstBufferData), 0)) {
			assert(false && "カメラ用コンスタントバッファの作成に失敗しました");
			return false;
		}

		// 三角形ポリゴン用コンスタントバッファの生成
		// 三角形ポリゴン用はディスクリプタヒープの2番目に作成
		if (!trianglePolygonConstantBufferInstance_.create(dx12Instance_, constantBufferDescriptorHeapInstance_, sizeof(Triangle_Polygon::ConstBufferData), 1)) {
			assert(false && "三角形ポリゴン用コンスタントバッファの作成に失敗しました");
			return false;
		}


		return true;
	}

	//アプリケーションループ
	void loop() noexcept {
		while (windowInstance_.messageLoop()) {

			//カメラの更新
			cameraInstance_.update();
			//三角形オブジェクトの更新
			triangleObjectInstance_.update();
			//四角形オブジェクトの更新
			squareObjectInstance_.update();

			//現在のバックバッファインデックスを取得
			const auto backBufferIndex = dx12Instance_.getSwapChain()->GetCurrentBackBufferIndex();

			//以前のフレームのGPUの処理が完了しているか確認する
			if (frameFenceValue_[backBufferIndex] != 0) {
				fenceInstance_.wait(frameFenceValue_[backBufferIndex]);
			}

			//コマンドアロケータリセット
			commandAllocatorInstance_[backBufferIndex].reset();
			//コマンドリストリセット
			commandListInstance_.reset(commandAllocatorInstance_[backBufferIndex]);

			//リソースバリアでレンダーターゲットをpresentからRenderTargetへ変更
			auto pToRT = resourceBarrier(renderTargetInstance_.get(backBufferIndex), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
			commandListInstance_.get()->ResourceBarrier(1, &pToRT);

			//レンダーターゲットの設定
			D3D12_CPU_DESCRIPTOR_HANDLE handles[] = { renderTargetInstance_.getDescriptorHandle(dx12Instance_, descriptorHeapInstance_, backBufferIndex) };
			commandListInstance_.get()->OMSetRenderTargets(1, handles, false, nullptr);

			//レンダーターゲットのクリア
			const float clearColor[] = { 1.0f, 0.75f, 0.80f, 1.0f };	//赤色でクリア
			commandListInstance_.get()->ClearRenderTargetView(handles[0], clearColor, 0, nullptr);

			// パイプラインステートの設定
			commandListInstance_.get()->SetPipelineState(piplineStateObjectInstance_.get());

			// ルートシグネチャの設定
			commandListInstance_.get()->SetGraphicsRootSignature(rootSignatureInstance_.get());

			// ビューポートの設定
			const auto [w, h] = windowInstance_.size();
			D3D12_VIEWPORT viewport{};
			viewport.TopLeftX = 0.0f;
			viewport.TopLeftY = 0.0f;
			viewport.Width = static_cast<float>(w);
			viewport.Height = static_cast<float>(h);
			viewport.MinDepth = 0.0f;
			viewport.MaxDepth = 1.0f;
			commandListInstance_.get()->RSSetViewports(1, &viewport);

			// シザー矩形の設定
			D3D12_RECT scissorRect{};
			scissorRect.left = 0;
			scissorRect.top = 0;
			scissorRect.right = w;
			scissorRect.bottom = h;
			commandListInstance_.get()->RSSetScissorRects(1, &scissorRect);

			// コンスタントバッファ用ディスクリプタヒープの設定
			ID3D12DescriptorHeap* p[] = { constantBufferDescriptorHeapInstance_.get() };
			commandListInstance_.get()->SetDescriptorHeaps(1, p);

			// カメラのコンスタントバッファへデータ転送
			Camera::ConstBufferData cameraData{
				DirectX::XMMatrixTranspose(cameraInstance_.viewMatrix()),
				DirectX::XMMatrixTranspose(cameraInstance_.projection()),
			};
			UINT8* pCameraData{};
			cameraConstantBufferInstance_.constantBuffer()->Map(0, nullptr, reinterpret_cast<void**>(&pCameraData));
			memcpy_s(pCameraData, sizeof(cameraData), &cameraData, sizeof(cameraData));
			cameraConstantBufferInstance_.constantBuffer()->Unmap(0, nullptr);
			commandListInstance_.get()->SetGraphicsRootDescriptorTable(0, cameraConstantBufferInstance_.getGpuDescriptorHandle());

			// 三角形ポリゴンのコンスタントバッファへデータ転送
			Triangle_Polygon::ConstBufferData triangleData{
				DirectX::XMMatrixTranspose(triangleObjectInstance_.world()),
				triangleObjectInstance_.color() };
			UINT8* pTriangleData{};
			trianglePolygonConstantBufferInstance_.constantBuffer()->Map(0, nullptr, reinterpret_cast<void**>(&pTriangleData));
			memcpy_s(pTriangleData, sizeof(triangleData), &triangleData, sizeof(triangleData));
			trianglePolygonConstantBufferInstance_.constantBuffer()->Unmap(0, nullptr);
			commandListInstance_.get()->SetGraphicsRootDescriptorTable(1, trianglePolygonConstantBufferInstance_.getGpuDescriptorHandle());

			// 四角形ポリゴンのコンスタントバッファへデータ転送
			Square_Polygon::ConstBufferData squareData{
				DirectX::XMMatrixTranspose(squareObjectInstance_.world()),
				squareObjectInstance_.color() };
			UINT8* pSquareData{};
			squarePolygonConstantBufferInstance_.constantBuffer()->Map(0, nullptr, reinterpret_cast<void**>(&pSquareData));
			memcpy_s(pSquareData, sizeof(squareData), &squareData, sizeof(squareData));
			squarePolygonConstantBufferInstance_.constantBuffer()->Unmap(0, nullptr);
			commandListInstance_.get()->SetGraphicsRootDescriptorTable(1, squarePolygonConstantBufferInstance_.getGpuDescriptorHandle());


			// ポリゴンの描画
			squarePolygonInstance_.draw(commandListInstance_);
			trianglePolygonInstance_.draw(commandListInstance_);

			//リソースバリアでレンダーターゲットをRenderTargetからPresentへ変更
			auto rtToP = resourceBarrier(renderTargetInstance_.get(backBufferIndex), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
			commandListInstance_.get()->ResourceBarrier(1, &rtToP);

			//コマンドリストをクローズ
			commandListInstance_.get()->Close();

			//コマンドキューにコマンドリストを送信
			ID3D12CommandList* ppCommandLists[] = { commandListInstance_.get() };
			dx12Instance_.getCommandQueue()->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

			//プレゼント
			dx12Instance_.getSwapChain()->Present(1, 0);

			//フェンスにフェンス値を設定
			dx12Instance_.getCommandQueue()->Signal(fenceInstance_.get(), nextFenceValue_);
			frameFenceValue_[backBufferIndex] = nextFenceValue_;
			nextFenceValue_++;
		}
		//ループを抜けるとウィンドウを閉じる
	}

	//リソースにバリアを設定する
	D3D12_RESOURCE_BARRIER resourceBarrier(ID3D12Resource* resource, D3D12_RESOURCE_STATES from, D3D12_RESOURCE_STATES to) noexcept {
		D3D12_RESOURCE_BARRIER barrier{};
		barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		barrier.Transition.pResource = resource;
		barrier.Transition.StateBefore = from;
		barrier.Transition.StateAfter = to;
		barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

		return barrier;
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

	RootSignature      rootSignatureInstance_{};  
	Shader             shaderInstance_{}; 
	PiplineStateObject piplineStateObjectInstance_{}; 
	DescriptorHeap	   constantBufferDescriptorHeapInstance_{};
	
	Triangle_Polygon    trianglePolygonInstance_{};
	Object				triangleObjectInstance_{};
	ConstantBuffer		trianglePolygonConstantBufferInstance_{};

	Square_Polygon		squarePolygonInstance_{};
	Object				squareObjectInstance_{};
	ConstantBuffer		squarePolygonConstantBufferInstance_{};

	Camera				cameraInstance_{};
	ConstantBuffer		cameraConstantBufferInstance_{};
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






































