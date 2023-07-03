#pragma once
#include "../camera/Camera.h"

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include <string>

using namespace Microsoft::WRL;
using namespace DirectX;

class Object3DSingleLine
{
public:

	//頂点構造体(3Dオブジェクト用)
	struct Vertex
	{
		XMFLOAT3 pos;
	};

	//定数バッファ
	struct ConstBufferDataTransform
	{
		XMMATRIX viewproj;
		XMMATRIX matStart;
		XMMATRIX matEnd;
		XMFLOAT4 color;
	};

public:
	Object3DSingleLine();
	~Object3DSingleLine();

	//静的関数
	static void CreateGraphicsPipeline();

	//メンバ関数
	void init();

	void updata();

	void draw();

	void setPoints(XMFLOAT3 start, XMFLOAT3 end)
	{
		startPoint = start;
		endPoint = end;
	}

private:

	//パイプラインステート
	static ComPtr<ID3D12RootSignature> rootsignature;
	static ComPtr<ID3D12PipelineState> pipelinestate;

	//定数バッファ
	ComPtr<ID3D12Resource> constBuff;

	//色
	XMFLOAT4 color = { 1,1,1,1 };

	//2点
	XMFLOAT3 startPoint = { 0,0,0 };
	XMFLOAT3 endPoint = { 0,0,0 };

	//行列
	XMMATRIX matStart = {};
	XMMATRIX matEnd = {};

};