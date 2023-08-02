#pragma once
#include"../Base/WindowGenerate.h"
#include"../Base/DirectX_Base.h"
#include"../camera/Camera.h"
#include"../staging/Easing.h"
#include<DirectXTex.h>
#include <d3dcompiler.h>
#include<string>
#include<map>

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
		XMFLOAT4 color;
	};

public:
	static void particleStaticInit(directX* Directx, Camera* Camera);

	static void setCamera(Camera* setcamera);

	static void createPipeline();

	static void createPipelineAddBlend();

	static void loadTexInMap(const std::string& filepath);

	SingleParticle();
	~SingleParticle();

	void generate();

	void loadTexture(const std::string& filepath);

	/// <summary>
	/// セッティング
	/// </summary>
	/// <param name="life">寿命</param>
	/// <param name="position">初期位置</param>
	/// <param name="velocity">移動する方向</param>
	/// <param name="accel">加速度</param>
	/// <param name="start_scale">初期サイズ</param>
	/// <param name="end_scale">最終サイズ</param>
	/// <param name="isextinctionframe">寿命で消えるかどうか</param>
	/// <param name="issetpos">インスタンス側で座標をセットするかどうか</param>
	void set(int life, XMFLOAT3 position, XMFLOAT3 velocity, XMFLOAT3 accel, float start_scale, float end_scale, bool isextinctionframe = true, bool issetpos = false);

	static void UpdateViewMatrix();

	void updata();

	void setPosition(XMFLOAT3 setposition)
	{
		if (!isSetPosition)
		{
			return;
		}

		position = setposition;
	}

	void draw();

	void drawSpecifyTex(const std::string texturename);

	void setPiplineAddBlend();

	void resetPipeline();

	void setIsActive(bool isactive) { isActive = isactive; }

	bool getIsActive() { return isActive; }

public:
	//座標
	XMFLOAT3 position = {};
	//速度
	XMFLOAT3 velocity = {};
	//加速度
	XMFLOAT3 accel = {};
	//色
	XMFLOAT4 color = { 1,1,1,1 };
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
	//消滅するかどうか
	bool isActive = false;
	//消滅する条件がフレームかどうか
	bool isExtinctionFrame = true;
	//インスタンス側でpositionをセットするかどうか
	bool isSetPosition = false;
	//加算合成するかどうか
	bool isAddBlend = false;
	// 定数バッファ
	ComPtr<ID3D12Resource> constBuff;
	// テクスチャバッファ
	ComPtr<ID3D12Resource> texbuff;
	// デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeap;
	// 頂点バッファ
	ComPtr<ID3D12Resource> vertBuff;
	// 頂点データ配列
	VertexPos vertices = {};
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView = {};

private:
	// 静的メンバ変数
	// デバイス
	static directX* directx;
	// ルートシグネチャ
	static ComPtr<ID3D12RootSignature> rootSignature;
	static ComPtr<ID3D12RootSignature> rootSignatureAddBlend;
	// パイプラインステートオブジェクト
	static ComPtr<ID3D12PipelineState> pipeLineState;
	static ComPtr<ID3D12PipelineState> pipeLineStateAddBlend;
	// ビュー行列
	static XMMATRIX matView;
	//ビルボード行列
	static XMMATRIX matBillboard;
	//Y軸周りビルボード行列
	static XMMATRIX matBillboardY;
	//カメラ
	static Camera* camera;

	//テクスチャ格納マップ
	static std::map<std::string, ComPtr<ID3D12DescriptorHeap>> texDescMap;
	static std::map<std::string, ComPtr<ID3D12Resource>> texBufMap;

	//通常時以外のパイプラインをセットしているか
	bool isSetOtherPipeline = false;
};