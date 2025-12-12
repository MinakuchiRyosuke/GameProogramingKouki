#pragma once
#include"Dx12.h"
#include"command_list.h"

class Triangle_Polygon final
{
public:
	Triangle_Polygon() = default;
	~Triangle_Polygon();

	//ポリゴンの生成
	[[nodiscard]] bool create(const Dx12& dx12) noexcept;

	//ポリゴンの描画
	[[nodiscard]] void draw(const CommandList& commandList) noexcept;

private:
	//頂点バッファの生成
	[[nodiscard]] bool createVertexBuffer(const Dx12& dx12) noexcept;

	//インデックスバッファの生成
	[[nodiscard]] bool createIndexBuffer(const Dx12& dx12) noexcept;

private:
	ID3D12Resource* vertexBuffer_{};	//頂点バッファ
	ID3D12Resource* indexBuffer_{};		//インデックスバッファ

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_ = {};	///頂点バッファビュー
	D3D12_INDEX_BUFFER_VIEW indexBufferView_ = {};		///インデックスバッファビュー
};

