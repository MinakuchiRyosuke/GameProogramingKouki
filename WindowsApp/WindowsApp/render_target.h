#pragma once
#include "Dx12.h"
#include "descriptor_heap.h"
#include<vector>

class RenderTarget final {
public:
	RenderTarget() = default;
	~RenderTarget();

	[[nodiscard]] bool createBackBuffer(const Dx12& dx12, const Dx12& swapChain, const DescriptorHeap& heap) noexcept;
	[[nodiscard]] D3D12_CPU_DESCRIPTOR_HANDLE getDescriptorHandle(const Dx12& dx12, const DescriptorHeap& heap, UINT index)const noexcept;
	[[nodiscard]] ID3D12Resource* get(uint32_t index) const noexcept;

private:
	std::vector<ID3D12Resource*> renderTargets_;
};