#pragma once
#include"SpriteSingleunit.h"
#include"../Base/DirectX_Base.h"

class PostEffect
{
public:
	//コンストラクタ
	PostEffect();

	//初期化
	void Init(ID3D12Device* dev);

	//描画
	void Draw(ID3D12GraphicsCommandList* cmdList, ID3D12Device* dev);

	//描画前処理
	void PreDrawScene(ID3D12GraphicsCommandList* cmdlist, ID3D12Device* dev);

	//描画後処理
	void PostDrawScene(ID3D12GraphicsCommandList* cmdlist, directX* directx);

	//深度バッファクリア
	void depthClear(ID3D12GraphicsCommandList* cmdlist);

	//パイプライン生成
	void CreateGraphicsPipelineState(ID3D12Device* dev);

	//通常
	ComPtr<ID3D12PipelineState> PostPipelinestate;//パイプラインステート
	ComPtr<ID3D12RootSignature> PostRootsignature;//ルートシグネチャ

	ComPtr<ID3D12Resource> texbuff[2];

	ComPtr<ID3D12DescriptorHeap> descHeapSRV;

	ComPtr<ID3D12Resource> depthbuff;

	ComPtr<ID3D12DescriptorHeap> descHeapRTV;

	ComPtr<ID3D12DescriptorHeap> descheapDSV;

	D3D12_CPU_DESCRIPTOR_HANDLE dsvH;

	//頂点バッファ
	ComPtr<ID3D12Resource> PostVertBuff;

	//頂点バッフビュー
	D3D12_VERTEX_BUFFER_VIEW PostVBView{};

	//定数バッファ
	ComPtr<ID3D12Resource> PostConstBuff;

	static const float clearColor[4];
};

