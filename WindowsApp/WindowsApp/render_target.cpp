//レンダーターゲット制御クラス
#include"render_target.h"
#include<cassert>

//デストラクタ
RenderTarget::~RenderTarget() {
	//レンダーターゲットリソースの解放
	for (auto& rt : renderTargets_) {
		if (rt) {
			rt->Release();
			rt = nullptr;
		}
	}
	renderTargets_.clear();
}

//---------------------------------------------------------------------------------
/**
 * @brief	バックバッファを生成する
 * @param	device		デバイスクラスのインスタンス
 * @param	swapChain	スワップチェインのポインタ
 * @param	heap		ディスクリプターヒープのインスタンス
 * @return	生成の成否
 */
[[nodiscard]] bool RenderTarget::createBackBuffer(const Dx12& dx12, const DescriptorHeap& heap)noexcept{
	//スワップチェインの設定を取得
	const auto& desc = dx12.swapChainDesc;
	//レンダーターゲットリソースのサイズを設定
	renderTargets_.resize(desc.BufferCount);
	//ディスクリプタヒープのハンドルを取得
	auto handle = heap.get()->GetCPUDescriptorHandleForHeapStart();
	//ディスクリプタヒープンタイプを取得
	auto heapType = heap.getType();
	assert(heapType == D3D12_DESCRIPTOR_HEAP_TYPE_RTV && "ディスクリプタヒープのタイプがRTVではありません");
	//バックバッファの生成
	for (uint8_t i = 0; i < desc.BufferCount; ++i) {
		const auto hr = dx12.swapChain->GetBuffer(i, IID_PPV_ARGS(&renderTargets_[i]));
		if (FAILED(hr)) {
			assert(false && "バックバッファの取得に失敗しました");
			return false;
		}
		//レンダーターゲットビューを作成してディスクリプタヒープのハンドルと関連付ける
		dx12.device->CreateRenderTargetView(renderTargets_[i], nullptr, handle);
		//次のハンドルへ移動
		handle.ptr += dx12.device->GetDescriptorHandleIncrementSize(heapType);
	}

	return true;
}

//---------------------------------------------------------------------------------
/**
 * @brief	ビュー（ディスクリプタハンドル）を取得する
 * @param	heap	ディスクリプタヒープのインスタンス
 * @param	index	インデックス
 * @return	ディスクリプタハンドル
 */
[[nodiscard]] D3D12_CPU_DESCRIPTOR_HANDLE RenderTarget::getDescriptorHandle(const Dx12& dx12, const DescriptorHeap& heap, UINT index)const noexcept {
	if (index >= renderTargets_.size() || !renderTargets_[index]) {
		assert(false && "不正なレンダーターゲットです");
	}
	//ディスクリプタヒープのハンドルを取得
	auto handle = heap.get()->GetCPUDescriptorHandleForHeapStart();
	//ディスクリプタヒープのタイプを取得
	auto heapType = heap.getType();
	assert(heapType == D3D12_DESCRIPTOR_HEAP_TYPE_RTV && "ディスクリプタヒープのタイプがRTVではありません");
	//インデックスに応じてハンドルを移動
	handle.ptr += index * dx12.device->GetDescriptorHandleIncrementSize(heapType);
	return handle;
}

//---------------------------------------------------------------------------------
/**
 * @brief	レンダーターゲットを取得する
 * @param	index	インデックス
 */
[[nodiscard]] ID3D12Resource* RenderTarget::get(uint32_t index)const noexcept {
	if (index >= renderTargets_.size() || !renderTargets_[index]) {
		assert(false && "不正なレンダーターゲットです");
		return nullptr;
	}
	return renderTargets_[index];
}