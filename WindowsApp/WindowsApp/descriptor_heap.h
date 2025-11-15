#pragma once

#include "device.h"

//ディスクリプタヒープ制御クラス
class DescriptorHeap final {
public:
	//コンストラクタ
	DescriptorHeap() = default;
	//デストラクタ
	~DescriptorHeap();
};