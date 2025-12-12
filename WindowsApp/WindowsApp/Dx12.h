#pragma once
#include <d3d12.h>
#include <dxgi1_4.h>
#include "window.h"

class Dx12 {
public:
    // コンストラクタ
    Dx12() = default;
    // デストラクタ
    ~Dx12();

    [[nodiscard]] bool setDisplayAdapter() noexcept;
    [[nodiscard]] IDXGIFactory4* factory() const noexcept;
    [[nodiscard]] IDXGIAdapter1* displayAdapter() const noexcept;
    [[nodiscard]] bool create(const Dx12& dxgi) noexcept;
    [[nodiscard]] ID3D12Device* getDevice() const noexcept;
    [[nodiscard]] bool create(const Dx12& device)noexcept;
    [[nodiscard]] ID3D12CommandQueue* getQueue() const noexcept;
    [[nodiscard]] bool create(const Dx12& dxgi, const Window& window, const Dx12& commandQueue)noexcept;
    [[nodiscard]] IDXGISwapChain3* getSwapChain() const noexcept;
    [[nodiscard]] const DXGI_SWAP_CHAIN_DESC1& getDesc() const noexcept;
private:
    IDXGIFactory4* dxgiFactory_{};
    IDXGIAdapter1* dxgiAdapter_{};
    ID3D12Device* device_;
    ID3D12CommandQueue* commandQueue_{};
    IDXGISwapChain3* swapChain_{};      
    DXGI_SWAP_CHAIN_DESC1 swapChainDesc_{};
};
