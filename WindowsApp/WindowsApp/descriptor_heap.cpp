#include "descriptor_heap.h"
#include <cassert>

//デストラクタ
DescriptorHeap::~DescriptorHeap() {
	//ディスクリプタヒープの解放
	if (heap_) {
		heap_->Release();
		heap_ = nullptr;
	}
}
[[nodiscard]] bool DescriptorHeap::create(const Dx12& dx12, D3D12_DESCRIPTOR_HEAP_TYPE type, UINT numDescriptors, bool shaderVisible)noexcept {
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
	heapDesc.Type						= type;
	heapDesc.NumDescriptors				= numDescriptors;
	heapDesc.Flags = shaderVisible ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

	type_ = type;

	HRESULT hr = dx12.device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&heap_));
	if (FAILED(hr)) {
		assert(false && "ディスクリプタヒープの生成に失敗しました");
		return false;
	}
	return true;
}

[[nodiscard]] ID3D12DescriptorHeap* DescriptorHeap::get() const noexcept {
	if (!heap_) {
		assert(false && "ディスクリプタヒープが未完成です");
		return nullptr;
	}
	return heap_;
}

[[nodiscard]] D3D12_DESCRIPTOR_HEAP_TYPE DescriptorHeap::getType() const noexcept {
	if (!heap_) {
		assert(false && "ディスクリプタヒープが未完成です");
	}
	return type_;
}