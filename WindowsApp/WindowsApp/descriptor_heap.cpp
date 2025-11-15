#include "descriptor_heap.h"
#include <cassert>

//デストラクタ
DescriptorHeap::~DescriptorHeap() {
	//ディスクリプタヒープの解放
	if(heap_)
}