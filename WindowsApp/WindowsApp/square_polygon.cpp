#include "square_polygon.h"
#include <cassert>
#include <DirectXMath.h>

namespace {
//頂点バッファフォーマット
struct Vertex {
	DirectX::XMFLOAT3 position;	//頂点座標
	DirectX::XMFLOAT4 color;	//頂点色
};
}

Square_Polygon::~Square_Polygon() {
	//頂点バッファの解放
	if (vertexBuffer_) {
		vertexBuffer_->Release();
		vertexBuffer_ = nullptr;
	}
	//インデックスバッファの解放
	if (indexBuffer_) {
		indexBuffer_->Release();
		indexBuffer_ = nullptr;
	}
}

//ポリゴンの生成
[[nodiscard]] bool Square_Polygon::create(const Dx12& dx12) noexcept {
	//頂点バッファ
	if (!createVertexBuffer(dx12)) {
		return false;
	}
	if (!createIndexBuffer(dx12)) {
		return false;
	}
	return true;
}

//頂点バッファの生成
[[nodiscard]] bool Square_Polygon::createVertexBuffer(const Dx12& dx12) noexcept {
	//今回利用する四角形の頂点データ
	Vertex squareVertices[] = {
		{   {-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}}, //左上頂点（赤色）
		{  {-0.5f, -0.5f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}}, //左下頂点（赤色）
		{ {0.5f, -0.5f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}}, //右下頂点（赤色）
		{{0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}}  //右上頂点（赤色）
	};

	//頂点データのサイズ
	const auto vertexBufferSize = sizeof(squareVertices);

	//ヒープの設定を指定
	//CPU～アクセス可能なメモリを利用するための設定
	D3D12_HEAP_PROPERTIES heapProperty{};
	heapProperty.Type = D3D12_HEAP_TYPE_UPLOAD;
	heapProperty.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProperty.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	heapProperty.CreationNodeMask = 1;
	heapProperty.VisibleNodeMask = 1;

	//どんなリソースを設定するかの設定
	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resourceDesc.Alignment = 0;
	resourceDesc.Width = vertexBufferSize;
	resourceDesc.Height = 1;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.MipLevels = 1;
	resourceDesc.Format = DXGI_FORMAT_UNKNOWN;
	resourceDesc.SampleDesc.Count = 1;
	resourceDesc.SampleDesc.Quality = 0;
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

	//頂点バッファの生成
	auto res = dx12.getDevice()->CreateCommittedResource(
		&heapProperty,
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertexBuffer_)
	);
	if (FAILED(res)) {
		assert(false && "頂点バッファの作成に失敗");
		return false;
	}

	//頂点バッファに出＾他を転送する
	//CPUからアクセスかぬなアドレスを取得
	Vertex* data{};

	//バッファをマップ（CPUからアクセス可能にする）
	//vertexBuffer_ を利用宇するのではなくdataを介して更新するイメージ
	res = vertexBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&data));
	if (FAILED(res)) {
		assert(false && "頂点バッファのマップに失敗");
		return false;
	}

	//頂点バッファをコピー
	memcpy_s(data, vertexBufferSize, squareVertices, vertexBufferSize);

	//コピーが終わったのでマップ解除（CPUからアクセス不可にする）
	//ここまで来たらGPUが利用するメモリ領域（VRAM）にコピー済みなのでsquareVertcesは不要になる
	vertexBuffer_->Unmap(0, nullptr);

	//頂点バッファビューの設定
	vertexBufferView_.BufferLocation = vertexBuffer_->GetGPUVirtualAddress();	//頂点バッファのアドレス
	vertexBufferView_.SizeInBytes = vertexBufferSize;
	vertexBufferView_.StrideInBytes = sizeof(Vertex);

	return true;
}

//インデックスバッファの生成
[[nodiscard]] bool Square_Polygon::createIndexBuffer(const Dx12& dx12) noexcept {
	uint16_t squareIndices[] = {
		//四角形を構成する頂点のインデックス
		0, 2, 1	,	//左下の三角形
		0, 3, 2		//右上の三角形
	};

	//インデックスデータのサイズ
	const auto indexBufferSize = sizeof(squareIndices);

	//ヒープの設定を指定
	D3D12_HEAP_PROPERTIES heapProperty{};
	heapProperty.Type = D3D12_HEAP_TYPE_UPLOAD;
	heapProperty.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProperty.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	heapProperty.CreationNodeMask = 1;
	heapProperty.VisibleNodeMask = 1;

	//リソースの設定を行う
	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resourceDesc.Alignment = 0;
	resourceDesc.Width = indexBufferSize;
	resourceDesc.Height = 1;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.MipLevels = 1;
	resourceDesc.Format = DXGI_FORMAT_UNKNOWN;
	resourceDesc.SampleDesc.Count = 1;
	resourceDesc.SampleDesc.Quality = 0;
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

	//インデックスバッファの生成
	auto res = dx12.getDevice()->CreateCommittedResource(
		&heapProperty,
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuffer_)
	);
	if (FAILED(res)) {
		assert(false && "インデックスバッファの作成に失敗");
		return false;
	}

	//インデックスバッファにデータを転送する
	uint16_t* data{};
	res = indexBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&data));
	if (FAILED(res)) {
		assert(false && "インデックスバッファのマップに失敗");
		return false;
	}

	memcpy_s(data, indexBufferSize, squareIndices, indexBufferSize);
	//ここまで来たらGPUが利用するメモリ領域（VRAM）にコピー済みなのでsquareIndicesは不要になる
	indexBuffer_->Unmap(0, nullptr);

	//インデックスバッファビュー作成
	indexBufferView_.BufferLocation = indexBuffer_->GetGPUVirtualAddress();
	indexBufferView_.SizeInBytes = indexBufferSize;
	indexBufferView_.Format = DXGI_FORMAT_R16_UINT;	//squareIndicesの型が16bit符号なし整数なのでR16_UINT

	return true;
}

//ポリゴンの描画
[[nodiscard]] void Square_Polygon::draw(const CommandList& commandList) noexcept {
	//頂点バッファの設定
	commandList.get()->IASetVertexBuffers(0, 1, &vertexBufferView_);
	//インデックスバッファの設定
	commandList.get()->IASetIndexBuffer(&indexBufferView_);
	//プリミティブ形状の設定(四角形）
	commandList.get()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//描画コマンド
	commandList.get()->DrawIndexedInstanced(6, 1, 0, 0, 0);
}