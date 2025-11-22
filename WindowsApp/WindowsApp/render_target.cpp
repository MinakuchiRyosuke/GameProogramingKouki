#include"render_target.h"
#include<cassert>

RenderTarget::~RenderTarget() {
	for (auto& rt : renderTargets_) {
		if (rt) {
			rt->Release();
			rt = nullptr;
		}
	}
	renderTargets_.clear();

	[[nodiscard]] bool RenderTarget::createBackBuffer(const Dx12& dx12, const Dx12& swapchain, const DescriptorHeap& heap)noexcept; {
		const auto& desc = swapchain.swapChain;
		renderTargets_.resize(desc.BufferCount);
		auto handle = heap.get()->GetCPUDescriptorHandleForHeapAtart();
		auto heapType = heap.getType();
		assert(heapType == D3D12_DESCRIPTOR_HEAP_TYPE_RTV && "ディスクリプタヒープのタイプがRTVではありません");

		for (uint8_t i = 0; i < desc.BufferCount; ++i) {
			const auto hr = swapChain->GetBuffer(i, IID_PPV_ARGS(&renderTargets_[i]));
			if (FAILED(hr)) {
				assert(false && "バックバッファの取得に失敗しました");
				return false;
			}
			dx12.device->CreateRenderTargetView(renderTargets_[i], nullptr, handle);
		}
	}
}