#pragma once

#include "Dx12.h"
#include "shader.h"
#include "root_signature.h"
class PiplineStateObject final
{
public:
	PiplineStateObject() = default;
	~PiplineStateObject();

	//パイプラインステートオブジェクトを作成する
	[[nodiscard]] bool create(const Dx12& dx12, const Shader& shader, const RootSignature& rootSignature) noexcept;

	//パイプラインステートを取得する
	[[nodiscard]] ID3D12PipelineState* get() const noexcept;

private:
	ID3D12PipelineState* pipelineState_ = {};	///パイプラインステート
};

