#pragma once
#include"Sprite.h"
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

	void setPipelineGray(ID3D12GraphicsCommandList* cmdList);
	void setPipelineGaussian(ID3D12GraphicsCommandList* cmdList);

	void reSetPipeline();

	//描画前処理
	void preDrawScene(ID3D12GraphicsCommandList* cmdlist, ID3D12Device* dev);

	//描画後処理
	void postDrawScene(ID3D12GraphicsCommandList* cmdlist, directX* directx);

	//深度バッファクリア
	void depthClear(ID3D12GraphicsCommandList* cmdlist);

private:

	//パイプライン生成
	void createGraphicsPipelineState(ID3D12Device* dev);//通常
	void createGraphicsPipelineStateGray(ID3D12Device* dev);//グレースケール
	void createGraphicsPipelineStateGaussian(ID3D12Device* dev);//ガウシアンブラー

	//通常
	ComPtr<ID3D12PipelineState> postPipelinestateBace;//パイプラインステート
	ComPtr<ID3D12RootSignature> postRootsignatureBace;//ルートシグネチャ

	ComPtr<ID3D12PipelineState> postPipelinestateGray;//パイプラインステート
	ComPtr<ID3D12RootSignature> postRootsignatureGray;//ルートシグネチャ

	ComPtr<ID3D12PipelineState> postPipelinestateGaussian;//パイプラインステート
	ComPtr<ID3D12RootSignature> postRootsignatureGaussian;//ルートシグネチャ

	ComPtr<ID3D12Resource> texbuff[2];

	ComPtr<ID3D12DescriptorHeap> descHeapSRV;

	ComPtr<ID3D12Resource> depthbuff;

	ComPtr<ID3D12DescriptorHeap> descHeapRTV;

	ComPtr<ID3D12DescriptorHeap> descheapDSV;

	D3D12_CPU_DESCRIPTOR_HANDLE dsvH;

	//頂点バッファ
	ComPtr<ID3D12Resource> postVertBuff;

	//頂点バッフビュー
	D3D12_VERTEX_BUFFER_VIEW postVBView{};

	//定数バッファ
	ComPtr<ID3D12Resource> postConstBuff;

	//クリアカラー
	static const float clearColor[4];

	//ほかのパイプラインをセットしているかどうか
	bool isSetOtherPipeline = false;
};

