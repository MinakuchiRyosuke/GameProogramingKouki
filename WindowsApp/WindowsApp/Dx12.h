#pragma once
#include <d3d12.h>
#include <dxgi1_4.h>

class Dx12 {
public:
    // コンストラクタ
    Dx12() = default;
    // デストラクタ
    ~Dx12();

    [[nodiscard]] bool setDisplayAdapter() noexcept;
    IDXGIFactory4* CreateDXGIFactory();
    IDXGIAdapter1* GetHardwareAdapter(IDXGIFactory4* factory);
    ID3D12Device* CreateD3D12Device(IDXGIAdapter1* adapter);
    ID3D12CommandQueue* CreateCommandQueue(ID3D12Device* device);
    IDXGISwapChain3* CreateSwapChain(IDXGIFactory4* factory, ID3D12CommandQueue* commandQueue, HWND hwnd);
    void                EnableDebugLayer();
    //ここで呼んでいるのは変数（先生のスクリプトは関数を呼び出している）()を付けえて呼べない
    ID3D12Device* device;
    IDXGISwapChain3* swapChain;     //ポインタ変数なので->を後ろにつけて関数を呼ぶ
    DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
};
