#pragma once

#include "Dx12.h"
class RootSignature final
{
public:
	RootSignature() = default;
	~RootSignature();

	//ルートシグネチャを作成する
	[[nodiscard]] bool create(const Dx12& dx12) noexcept;

	//ルートシグネチャを取得する
	[[nodiscard]] ID3D12RootSignature* get() const noexcept;

private:
	ID3D12RootSignature* rootSignature_{};	///ルートシグネチャ
};

