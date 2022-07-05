#pragma once
#include"SpriteCommon.h"
#include<DirectXTex.h>
#include<d3dx12.h>
#include<wrl.h>
#include<string>
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

//頂点構造体(スプライト用)
struct VertexPosUv
{
	XMFLOAT3 pos;
	XMFLOAT2 uv;
};

class SingleSprite
{
public:
	/// <summary>
	/// パイプライン生成
	/// </summary>
	static void SetPipelineStateSprite();

	/// <summary>
	/// 頂点バッファへのデータ転送
	/// </summary>
	/// <param name="tex">テクスチャマネージャー</param>
	/// <param name="isCutout">切り取るかどうか</param>
	void SpriteTransferVertexBuffer(TexManager* tex, bool isCutout);

	/// <summary>
	/// テクスチャ読み込み
	/// </summary>
	/// <param name="filename">ファイル名</param>
	void LoadTexture(const std::string& filename);

	/// <summary>
	/// スプライト生成
	/// </summary>
	/// <param name="dev">デバイス情報</param>
	/// <param name="texnumber">テクスチャ番号</param>
	/// <param name="tex">テクスチャマネージャー</param>
	/// <param name="sizeFlag">画像サイズに切り抜く</param>
	/// <param name="isFlipX">左右反転</param>
	/// <param name="isFlipY">上下反転</param>
	/// <param name="iscutout">切り抜くかどうか</param>
	void GenerateSprite(ID3D12Device* dev,
		UINT texnumber,
		TexManager* tex,
		bool sizeFlag = false,
		bool isFlipX = false,
		bool isFlipY = false,
		bool iscutout = false);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="spritecommon">スプライト共通データ</param>
	void SpriteUpdate(const SpriteCommon& spritecommon);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="cmdList">コマンドリスト</param>
	/// <param name="texture">テクスチャマネージャー</param>
	void DrawSprite(ID3D12GraphicsCommandList* cmdList, TexManager* texture);

public:
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

private:
	static ID3D12Device* device;

	static ComPtr<ID3D12RootSignature> SpriteRootsignature;//ルートシグネチャ

	static ComPtr<ID3D12PipelineState> SpritePipelinestate;//パイプラインステート

	//頂点バッファ
	ComPtr<ID3D12Resource> spriteVertBuff;

	//頂点バッフビュー
	D3D12_VERTEX_BUFFER_VIEW spriteVBView{};

	//定数バッファ
	ComPtr<ID3D12Resource> spriteConstBuff;

	//テクスチャバッファ
	ComPtr<ID3D12Resource> texbuff;

	//シェーダーリソースビュー
	ComPtr<ID3D12DescriptorHeap> descHeapSRV;
};