#pragma once
#include"SpriteCommon.h"
#include<DirectXTex.h>
#include<d3dx12.h>
#include<wrl.h>
#include<DirectXMath.h>
#include <d3dcompiler.h>
#include"../Base/WindowGenerate.h"
#include"../Base/TexManager.h"

#pragma comment(lib, "d3dcompiler.lib")

using XMFLOAT3 = DirectX::XMFLOAT3;

//定数バッファ用データ構造体
struct ConstBufferDataSP {
	XMFLOAT4 color;				//色　RGBA
	XMMATRIX mat;
};

//スプライト用グローバル変数
struct VertexPosUv										//頂点構造体(スプライト用)
{
	XMFLOAT3 pos;
	XMFLOAT2 uv;
};

class SingleSprite
{
public:
	//スプライト単体頂点バッファの転送
	void SpriteTransferVertexBuffer(TexManager* tex, bool isCutout);

	//スプライト生成
	void GenerateSprite(ID3D12Device* dev,
		float window_width,
		float window_heigh,
		UINT texnumber,
		TexManager* tex,
		bool sizeFlag = false,
		bool isFlipX = false,
		bool isFlipY = false,
		bool iscutout = false);

	//スプライト更新処理
	void SpriteUpdate(const SpriteCommon& spritecommon);

	//スプライト描画コマンド
	void DrawSprite(ID3D12GraphicsCommandList* cmdList, TexManager* texture, ID3D12Device* dev);

	//頂点バッファ
	ComPtr<ID3D12Resource> spriteVertBuff;

	//頂点バッフビュー
	D3D12_VERTEX_BUFFER_VIEW spriteVBView{};

	//定数バッファ
	ComPtr<ID3D12Resource> spriteConstBuff;

	//Z軸周りの回転角
	float rotation = 0.0f;

	//座標
	XMFLOAT3 position = { 0,0,0 };

	//ワールド行列
	XMMATRIX matWorld;

	//色(RGBA)
	XMFLOAT4 color = { 1,1,1,1 };

	//テクスチャ番号
	UINT texnumber = 0;

	//大きさ
	XMFLOAT2 size = { 100.0f,100.0f };

	//アンカーポイント
	XMFLOAT2 anchorpoint = { 0.0f,0.0f };

	//左右反転
	bool isFlipX = false;

	//上下反転
	bool isFlipY = false;

	//テクスチャ左上座標
	XMFLOAT2 texLeftTop = { 0,0 };

	//テクスチャ切り出しサイズ
	XMFLOAT2 texSize = { 100,100 };

	//非表示
	bool isInvisible = false;

public:

	static ComPtr<ID3D12PipelineState> SpritePipelinestate;//パイプラインステート

	static ComPtr<ID3D12RootSignature> SpriteRootsignature;//ルートシグネチャ

	//パイプライン設定
	static void SetPipelineStagte(ID3D12Device* dev);
};