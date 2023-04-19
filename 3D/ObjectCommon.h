#pragma once
#include"../Base/DirectX_Base.h"
#include"../Base/TexManager.h"
#include<DirectXMath.h>
#include<cmath>
#include<string>
#include <DirectXTex.h>
#include <cassert>
#include<vector>
#include<unordered_map>
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;
using namespace std;

//パイプラインセット
struct PipelineSet3D
{
	ComPtr<ID3D12PipelineState> pipelinestate;				//パイプラインステート

	ComPtr<ID3D12RootSignature> rootsignature;				//ルートシグネチャ
};

//頂点構造体(3Dオブジェクト用)
struct Vertex
{
	XMFLOAT3 pos;
	XMFLOAT3 normal;
	XMFLOAT2 uv;
};

//マテリアル構造体
struct Material
{
	std::string name;
	XMFLOAT3 ambient;
	XMFLOAT3 diffuse;
	XMFLOAT3 specular;
	float alpha;
	std::string textureFilename;
	//コンストラクタ
	Material()
	{
		ambient = { 0.3f,0.3f,0.3f };
		diffuse = { 0.0f,0.0f,0.0f };
		specular = { 0.0f,0.0f,0.0f };
		alpha = 1.0f;
	}

	// シェーダリソースビューのハンドル(CPU)
	CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
	// シェーダリソースビューのハンドル(CPU)
	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;

	int material_texnumber = 0;
};

//バッファビュー構造体
struct Viewes
{
	D3D12_VERTEX_BUFFER_VIEW vbView{};						//頂点バッファビュー
	D3D12_INDEX_BUFFER_VIEW idView{};						//インデックスバッファビュー
	//頂点バッファの生成
	ComPtr<ID3D12Resource> vertBuff;
	ComPtr<ID3D12Resource> indexBuff;
	//頂点インデックス配列_保存用
	unsigned short Cubeindices[36];
	unsigned short Triindices[18];
	unsigned short Planeindices[6];

	//モデル用頂点配列
	std::vector<Vertex> vertices;

	//モデル用インデックス配列
	std::vector<unsigned short> modelindices;

	//マテリアル情報
	Material material;
};


//3dオブジェクト共通クラス
class ObjectCommon
{
public:
	//定数バッファの最大数
	const int constantBufferNum = 2048;

	//定数バッファ用のデスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> basicDescHeap;

	//バッファビュー取得_立方体
	Viewes returnCubeViewes();

	//バッファビュー取得_三角錐
	Viewes returnTripyramidViewes();

	//バッファビュー取得_面
	Viewes returnPlaneViewes();

	//マテリアル用のテクスチャ読み込み
	void materialLoadTexture(Material* material, const std::string& filename);

	//マテリアル読み込み
	void loadMaterial(Material* material, const std::string& filename);

	//バッファビュー取得_モデル
	Viewes returnModelViewes(const char* Filename,int material_texnum);

	//パイプライン
	PipelineSet3D pipelineSet;

	//パイプライン設定
	PipelineSet3D  generatePipeLineState(ID3D12Device* dev);

	//初期化
	void init(directX* directx);

	//3Dオブジェクト共通描画処理
	void object3DcommonBeginDraw();

public:
	directX* directx;

	UINT descriptorHandleIncrementSize;

	ComPtr<ID3D12DescriptorHeap> materialDescHeap;

	ComPtr<ID3D12Resource> texbuff[512];
};