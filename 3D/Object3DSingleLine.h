#pragma once
#include "../camera/Camera.h"
#include"../Base/DirectX_Base.h"

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include <string>
#include <d3dcompiler.h>

using namespace Microsoft::WRL;
using namespace DirectX;

class Object3DSingleLine
{
public:

	//頂点構造体(3Dオブジェクト用)
	struct VertexPos
	{
		XMFLOAT3 pos;
	};

	//定数バッファ
	struct ConstBufferDataTransform
	{
		XMFLOAT4 color = { 1,1,1,1 };
		XMMATRIX viewproj;
		XMMATRIX startMat;
		XMMATRIX endMat;
	};

public:
	Object3DSingleLine();
	~Object3DSingleLine();

	//静的関数
	static void setStaticData(directX* dx);
	static void CreateGraphicsPipeline();

	/// <summary>
	/// カメラセット
	/// </summary>
	/// <param name="cmr">カメラのポインター</param>
	static void setCamera(Camera* cmr) { camera = cmr; }

	//メンバ関数
	void init();
	void lineTransferVertexBuffer();
	void updata();
	void draw();
	void setPoints(XMFLOAT3 start, XMFLOAT3 end)
	{
		startPoint = start;
		endPoint = end;
	}
	void setColor(XMFLOAT4 c)
	{
		color = c;
	}

private:

	//directxbase
	static directX* directx;

	//camera
	static Camera* camera;

	//パイプラインステート
	static ComPtr<ID3D12RootSignature> rootsignature;
	static ComPtr<ID3D12PipelineState> pipelinestate;

	//定数バッファ
	ComPtr<ID3D12Resource> constBuff;
	//頂点バッファ
	ComPtr<ID3D12Resource> vertBuff;
	//頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView{};

	//行列
	XMMATRIX matStart = {};
	XMMATRIX matEnd = {};

	//2点
	XMFLOAT3 startPoint = { 0,0,0 };
	XMFLOAT3 endPoint = { 0,0,0 };

	//色
	XMFLOAT4 color = { 1,1,1,1 };
};