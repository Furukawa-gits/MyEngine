#pragma once
#include"../Base/TexManager.h"
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

//パイプラインセット
struct PipelineSet
{
	ComPtr<ID3D12PipelineState> pipelinestate;//パイプラインステート

	ComPtr<ID3D12RootSignature> rootsignature;//ルートシグネチャ
};

class SpriteCommon
{
public:
	// スプライトの共通データ
 
	//パイプラインセット
	PipelineSet pipelineset;

	//射影行列
	XMMATRIX matProjection{};

	//パイプライン設定
	PipelineSet generatePipeLineStateSP(ID3D12Device* dev);

	//共通データ生成
	void spriteCommonCreate(ID3D12Device* dev , int window_width, int window_height);

	//スプライト共通グラフィックスコマンドのセット
	void spriteCommonBeginDraw(ID3D12GraphicsCommandList* cmdlist, TexManager* tex);
};