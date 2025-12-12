#include "Dx12.h"
#include <cassert>

#pragma comment(lib, "d3d12.lib") 
#pragma comment(lib, "dxgi.lib")

Dx12::~Dx12() {
    if (dxgiFactory_) {
        dxgiFactory_->Release();
        dxgiFactory_ = nullptr;
    }
    if (dxgiAdapter_) {
        dxgiAdapter_->Release();
        dxgiAdapter_ = nullptr;
    }
    if (device_) {
        device_->Release();
        device_ = nullptr;
    }
    if (commandQueue_) {
        commandQueue_->Release();
        commandQueue_ = nullptr;
    }
    if (swapChain_) {
        swapChain_->Release();
        swapChain_ = nullptr;
    }
}

[[nodiscard]] bool Dx12::setDisplayAdapter()noexcept {
#if _DEBUG
    ID3D12Debug* debug;
    if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debug)))) {
        debug->EnableDebugLayer();
    }
#endif

    //DXGIファクトリーの作成
    {
        UINT createFactoryFlags = 0;
#if _DEBUG
        createFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
#endif
        const auto hr = CreateDXGIFactory2(createFactoryFlags, IID_PPV_ARGS(&dxgiFactory_));
        if (FAILED(hr)) {
            assert(false && "DXGIファクトリーの作成に失敗");
            return false;
        }
    }

    //アダプタの取得
    {
        auto        select = 0;
        IDXGIAdapter1* dxgiAdapter{};

        while (dxgiFactory_->EnumAdapters1(select, &dxgiAdapter) != DXGI_ERROR_NOT_FOUND) {

            DXGI_ADAPTER_DESC1 desc{};
            dxgiAdapter->GetDesc1(&desc);

            select++;

            if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) {
                dxgiAdapter->Release();
                continue;
            }

            if (FAILED(D3D12CreateDevice(dxgiAdapter, D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr))) {
                dxgiAdapter->Release();
                continue;
            }

            dxgiAdapter_ = dxgiAdapter;
            break;
        }

        if (!dxgiAdapter_) {
            assert(false && "アダプタの取得に失敗");
            return false;
        }
    }
    return true;
}
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
[[nodiscard]] IDXGIFactory4* Dx12::factory() const noexcept{
    if (!dxgiFactory_) {
        assert(false && "DXGIファクトリーが未作成です");
        return nullptr;
    }
    return dxgiFactory_;
}

// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
[[nodiscard]]IDXGIAdapter1* Dx12::displayAdapter() const noexcept {
    if (!dxgiAdapter_) {
        assert(false && "ディスプレイアダプターが未作成です");
        return nullptr;
    }

    return dxgiAdapter_;
}

[[nodiscard]] bool Dx12::createDevice(const Dx12& dx12)noexcept {
    const auto hr = D3D12CreateDevice(dx12.displayAdapter(), D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&device_));
    if (FAILED(hr)) {
        assert(false && "デバイス作成に失敗");
        return false;
    }

    return true;
}
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
[[nodiscard]]ID3D12Device* Dx12::getDevice() const noexcept {
    if (!device_) {
        assert(false && "デバイスが未作成です");
        return nullptr;
    }

    return device_;
}

// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
[[nodiscard]]bool Dx12::createCommandQueue(const Dx12& dx12) noexcept{
    // コマンドキューの設定
    D3D12_COMMAND_QUEUE_DESC desc = {};
    desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;       // 直接実行型
    desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;  // 通常優先度
    desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;        // 特別フラグなし
    desc.NodeMask = 0;                                    // 単一GPU使用

    const auto hr = dx12.getDevice()->CreateCommandQueue(&desc, IID_PPV_ARGS(&commandQueue_));
    if (FAILED(hr)) {
        assert(false && "コマンドキューの作成に失敗");
        return false;
    }
    return true;
}

[[nodiscard]] ID3D12CommandQueue* Dx12::getCommandQueue() const noexcept {
    if (!commandQueue_) {
        assert(false && "コマンドキューが未作成です");
        return nullptr;
    }
    return commandQueue_;
}

// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
[[nodiscard]] bool Dx12::createSwapChain(const Dx12& dx12, const Window& window) noexcept{
    // スワップチェーンの詳細設定
    const auto [w, h] = window.size();

    swapChainDesc_ = {};
    swapChainDesc_.BufferCount = 2;                                // ダブルバッファリング
    swapChainDesc_.Width = w;                             // 画面幅
    swapChainDesc_.Height = h;                              // 画面高
    swapChainDesc_.Format = DXGI_FORMAT_R8G8B8A8_UNORM;       // ピクセルフォーマット
    swapChainDesc_.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;  // レンダーターゲット用
    swapChainDesc_.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;    // 高速切替
    swapChainDesc_.SampleDesc.Count = 1;                                // マルチサンプリングなし

    //一時的なスワップチェインの作成
    //スワップチェインのアップグレードが必要になる
    IDXGISwapChain1* tempSwapChain{};
    {
        const auto hr = dx12.factory()->CreateSwapChainForHwnd(dx12.getCommandQueue(), window.handle(), &swapChainDesc_, nullptr, nullptr, &tempSwapChain);

        if (FAILED(hr)) {
            assert(false && "スワップチェインの作成に失敗");
            return false;
        }
    }

    //スワップチェインのバージョンをアップグレード
    {
        //一時的なスワップチェインをIDXGISwapChain3に変換
        const auto hr = tempSwapChain->QueryInterface(IID_PPV_ARGS(&swapChain_));
        //一時的なスワップチェインは解放
        tempSwapChain->Release();

        if (FAILED(hr)) {
            assert(false && "スワップチェインのアップグレードに失敗");
            return false;
        }
    }
    return true;
}

//スワップチェインを取得する
[[nodiscard]] IDXGISwapChain3* Dx12:: getSwapChain() const noexcept {
    if (!swapChain_) {
        assert(false && "スワップチェインが未作成です");
        return nullptr;
    }
    return swapChain_;
}

//スワップチェインの設定を取得する
[[nodiscard]] const DXGI_SWAP_CHAIN_DESC1& Dx12::getDesc() const noexcept {
    if (!swapChain_) {
        assert(false && "スワップチェインが未作成です");
    }
    return swapChainDesc_;
}
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------
//void Dx12::EnableDebugLayer() {
//#if defined(_DEBUG)
//    // デバッグインターフェースを取得
//    ID3D12Debug* debugController;
//    if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
//        // デバッグレイヤーを有効化
//        debugController->EnableDebugLayer();
//
//        // より詳細な検証を有効化（任意）
//        ID3D12Debug1* debugController1;
//        if (SUCCEEDED(debugController->QueryInterface(IID_PPV_ARGS(&debugController1)))) {
//            debugController1->SetEnableGPUBasedValidation(TRUE);
//        }
//    }
//#endif
//}