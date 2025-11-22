#pragma once

#include "DX12.h"

//ディスクリプタヒープ制御クラス
class DescriptorHeap final {
public:
	//コンストラクタ
	DescriptorHeap() = default;
	//デストラクタ
	~DescriptorHeap();
	[[nodiscard]]bool create(const Dx12& dx12, D3D12_DESCRIPTOR_HEAP_TYPE type, UINT numDescriptors, bool shaderVisible = false) noexcept;
	[[nodiscard]] ID3D12DescriptorHeap* get() const noexcept;
	[[nodiscard]] D3D12_DESCRIPTOR_HEAP_TYPE getType() const noexcept;

private:
	ID3D12DescriptorHeap*		heap_{};
	D3D12_DESCRIPTOR_HEAP_TYPE  type_{};
};