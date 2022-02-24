#pragma once
#include"ObjectCommon.h"
#include"../camera/Camera.h"

//using namespace DirectX;

//定数バッファ用データ構造体
struct ConstBufferDataB0 {
	//XMFLOAT4 color;				//色　RGBA
	XMMATRIX mat;
};

struct ConstBufferDataB1
{
	XMFLOAT3 ambient;
	float pad1;
	XMFLOAT3 diffuse;
	float pad2;
	XMFLOAT3 specular;
	float alpha;
};

//実装済みのバッファービューに合わせて増えていく
enum indices_num
{
	cube,
	tri,
	plane
};

float returnScaler(XMFLOAT3 vec);

XMFLOAT3 normalized(XMFLOAT3 vec);

class object3D
{
public:

	//定数バッファ
	ComPtr<ID3D12Resource> constbuffB0;		//B0
	ComPtr<ID3D12Resource> constbuffB1;		//B1

	//定数バッファビューのハンドル(CPU)
	D3D12_CPU_DESCRIPTOR_HANDLE cpuDescHandleCBV;

	//定数バッファビューのハンドル(GPU)
	D3D12_GPU_DESCRIPTOR_HANDLE gpuDescHandleCBV;

	//アフィン変換情報
	XMFLOAT3 scale = { 1,1,1 };
	XMFLOAT3 rotation = { 0,0,0 };
	XMFLOAT3 position = { 0,0,0 };

	//ワールド変換行列
	XMMATRIX matWorld;

	//親オブジェクトへのポインタ
	object3D* parent = nullptr;

	//フラグ
	bool flag;

	//コンストラクタ
	object3D();

	//静的初期化
	static void StaticInit(Camera* camera = nullptr, directX* directx = nullptr);

	//静的データセット
	static void SetStaticData(Camera* camera, directX* directx, ObjectCommon* common);

	//3Dオブジェクト初期化
	void Init3d(int index);

	//3Dオブジェクト更新処理
	void Update3d(Viewes& view, XMFLOAT4 color = { 1,1,1,1 });

	//3Dオブジェクト描画処理
	void Draw3d(ID3D12DescriptorHeap* TexdescHeap, Viewes& view, D3D12_GPU_DESCRIPTOR_HANDLE gpuDeschandleSRV, indices_num num);

	//モデル専用描画処理(マテリアル無し)
	void DrawModel(ID3D12DescriptorHeap* TexdescHeap, Viewes& view, D3D12_GPU_DESCRIPTOR_HANDLE gpuDeschandleSRV);

	//モデル描画(マテリアルあり)
	void DrawModel_OnMaterial(Viewes& view);

	//ワールド座標からスクリーン座標
	XMVECTOR WorldToScreenPos(XMVECTOR World_pos);

public:
	//カメラ
	static Camera* camera;

	//directx基盤データ
	static directX* directx;

	//共通データ
	static ObjectCommon* objectcommon;
};