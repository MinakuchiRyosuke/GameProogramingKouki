#pragma once

#include "Dx12.h"
#include "descriptor_heap.h"

class ConstantBuffer final
{
public:
	ConstantBuffer() = default;
	~ConstantBuffer() = default;

	//コンスタントバッファの生成
	[[nodiscard]] bool create(const Dx12& dx12, const DescriptorHeap& descriptorHeap, UINT bufferSize, UINT descriptorIndex) noexcept;

	//コンスタントバッファを取得
	[[nodiscard]] ID3D12Resource* constantBuffer() const noexcept;

	//GPU用ディスククリプタハンドルを取得する
	[[nodiscard]] D3D12_GPU_DESCRIPTOR_HANDLE getGpuDescriptorHandle() const noexcept;

private:
	ID3D12Resource* constantBuffer_{};	///コンスタントバッファ本体
	D3D12_GPU_DESCRIPTOR_HANDLE	gpuHandle_{};	///GPU用ディスクリプタハンドル
};

