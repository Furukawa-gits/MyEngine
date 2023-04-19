#pragma once
#include<d3d12.h>
#include<dxgi1_6.h>
#include<wrl.h>
#include"WindowGenerate.h"
#include<vector>
#include<string>
#include<d3dx12.h>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

using namespace Microsoft::WRL;
using namespace std;

class directX
{
private:
	float clearColor[4] = { 0.2f,0.2f,0.8f,0 };
public:
	HRESULT result;
	ComPtr<IDXGIFactory6> dxgiFactory;
	ComPtr<IDXGISwapChain4> swapchain;
	ComPtr<ID3D12CommandAllocator> cmdAllocator;
	ComPtr<ID3D12GraphicsCommandList> cmdList;
	ComPtr<ID3D12CommandQueue> cmdQueue;
	ComPtr<ID3D12DescriptorHeap> rtvHeaps;
	vector<ComPtr<IDXGIAdapter1>> adapters;
	ComPtr<IDXGIAdapter1> tmpAdapter;
	ComPtr<ID3D12Device> dev;
	ComPtr<ID3D12Resource> backBuffers[2];
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};
	ComPtr<ID3D12Fence> fence;
	UINT64 fenceVal;
	UINT bbIndex;
	D3D12_CPU_DESCRIPTOR_HANDLE rtvH;
	D3D12_CPU_DESCRIPTOR_HANDLE dsvH;

	//深度バッファ
	//深度バッファのリソース
	CD3DX12_RESOURCE_DESC depthResDesc;

	//深度バッファのリソース
	ComPtr<ID3D12Resource> depthBuffer;

	//深度ビュー用デスクリプタヒープ
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	ComPtr<ID3D12DescriptorHeap> dsvHeap;

	//深度ビュー
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};

	//コンストラクタ
	directX();

	//初期化
	void Init(Window_Generate* Win);

	//深度バッファ初期化
	void initializeDepth();

	//描画処理前
	void preDraw();

	//描画処理後
	void postDraw();

	//深度バッファクリア
	void depthclear();

	//バッファのフリップ
	void finishDraw();
};