#pragma once

#include "Dx12.h"
#include "descriptor_heap.h"
#include "window.h"

class DepthBuffer final
{
public:
	DepthBuffer() = default;
	~DepthBuffer();

	//デプスバッファを生成
	[[nodiscard]] bool create(const Dx12& dx12, const DescriptorHeap& heap, const Window& window) noexcept;
};

