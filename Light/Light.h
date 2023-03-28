#pragma once
#include <wrl.h>
#include <DirectXMath.h>
#include <d3dx12.h>

class Light
{
private://エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	//定数バッファ構造体
	struct ConstBufferData
	{
		XMVECTOR lightv;		//ライトの方向
		XMFLOAT3 lightcolor;	//ライトの色
	};

private://静的メンバ変数
	static ID3D12Device* device;

public://静的メンバ関数

	/// <summary>
	/// 静的初期化
	/// </summary>
	/// <param name="device">デバイス</param>
	static void Staticinitialize(ID3D12Device* device);

	/// <summary>
	/// 定数バッファ転送
	/// </summary>
	void TransferConstBuffer();

	/// <summary>
	/// 初期化
	/// </summary>
	void init();

	/// <summary>
	/// ライト方向をセット
	/// </summary>
	/// <param name="lightdir">ライト方向</param>
	void SetLightDir(const XMVECTOR& lightdir);

	/// <summary>
	/// ライト色をセット
	/// </summary>
	/// <param name="lightcolor">ライト色</param>
	void SetLightColor(const XMFLOAT3& lightcolor);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ID3D12GraphicsCommandList* cmdlist, UINT rootParameterindex);

	/// <summary>
	/// インスタンス生成
	/// </summary>
	/// <returns>インスタンス</returns>
	static Light* Create();



private://メンバ変数
	//定数バッファ
	ComPtr<ID3D12Resource> constBuff;
	//ライト光線方向
	XMVECTOR lightdir = { 1,0,0,0 };
	//ライト色
	XMFLOAT3 lightcolor = { 1,1,1 };
	//ダーティフラグ
	bool dirty = false;

};