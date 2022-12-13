#pragma once
#include"../Base/WindowGenerate.h"
#include"../Base/DirectX_Base.h"
#include"../camera/Camera.h"
#include"../staging/Easing.h"
#include<DirectXTex.h>
#include <d3dcompiler.h>
#include<string>

#pragma comment(lib, "d3dcompiler.lib")

using namespace Microsoft::WRL;

class SingleParticle
{
private:
	// 頂点データ構造体
	struct VertexPosNormalUv
	{
		XMFLOAT3 pos; // xyz座標
		XMFLOAT3 normal; // 法線ベクトル
		XMFLOAT2 uv;  // uv座標
	};

	//頂点データ構造体(座標のみ)
	struct VertexPos
	{
		XMFLOAT3 pos;//xyz座標
		float scale;
	};

	// 定数バッファ用データ構造体
	struct ConstBufferData
	{
		XMMATRIX mat;	// ３Ｄ変換行列
		XMMATRIX matBillboard;
	};

public:
	static void particleStaticInit(directX* Directx, Camera* Camera);

	static void setCamera(Camera* setcamera);

	static void createPipeline();

	void generate(const std::string& filepath);

	void loadTexture(const std::string& filepath);

	void set(int life, XMFLOAT3 position, XMFLOAT3 velocity, XMFLOAT3 accel, float start_scale, float end_scale);

	static void UpdateViewMatrix();

	void updata();

	void draw();

public:
	//座標
	XMFLOAT3 position = {};
	//速度
	XMFLOAT3 velocity = {};
	//加速度
	XMFLOAT3 accel = {};
	//スケール
	float scale = 1.0f;
	//初期値
	float s_scale = 1.0f;
	//最終値
	float e_scale = 0.0f;
	//現在フレーム
	int frame = 0;
	//終了フレーム
	int num_frame = 0;
	// 定数バッファ
	ComPtr<ID3D12Resource> constBuff;
	// テクスチャバッファ
	ComPtr<ID3D12Resource> texbuff;
	// デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeap;
	// 頂点バッファ
	ComPtr<ID3D12Resource> vertBuff;
	// 頂点データ配列
	VertexPos vertices;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView;

private:
	// 静的メンバ変数
	// デバイス
	static directX* directx;
	// ルートシグネチャ
	static ComPtr<ID3D12RootSignature> rootsignature;
	// パイプラインステートオブジェクト
	static ComPtr<ID3D12PipelineState> pipelinestate;
	// ビュー行列
	static XMMATRIX matView;
	//ビルボード行列
	static XMMATRIX matBillboard;
	//Y軸周りビルボード行列
	static XMMATRIX matBillboardY;
	//カメラ
	static Camera* camera;
};