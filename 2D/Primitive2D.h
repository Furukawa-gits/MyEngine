#pragma once
#include"../Base/WindowGenerate.h"
//#include<DirectXTex.h>
#include<d3dx12.h>
#include<wrl.h>
//#include<string>
#include<DirectXMath.h>
#include <d3dcompiler.h>

#pragma comment(lib, "d3dcompiler.lib")

using namespace Microsoft::WRL;
using namespace DirectX;

//定数バッファ用データ構造体(線)
struct ConstBufferDataL {
	XMFLOAT4 color = { 1,1,1,1 };
	XMMATRIX startMat;
	XMMATRIX endMat;
};

//頂点構造体(線)
struct VertexPos
{
	XMFLOAT3 pos;
};

//線
class primitiveLine2D
{
	static void SetStaticData(ID3D12Device* dev);
	void createGP();
	void init();
	void update();
	void draw();

private:
	static ID3D12Device* device;

	static ComPtr<ID3D12RootSignature> SpriteRootsignature;//ルートシグネチャ

	static ComPtr<ID3D12PipelineState> SpritePipelinestate;//パイプラインステート

	static XMMATRIX matprojection;

	//頂点バッファ
	ComPtr<ID3D12Resource> spriteVertBuff;
	//定数バッファ
	ComPtr<ID3D12Resource> spriteConstBuff;
	//頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	//インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW idView{};

	XMFLOAT3 startPos = { 0,0,0 };
	XMFLOAT3 endPos = { 0,0,0 };

	XMFLOAT4 color = { 1,1,1,1 };
};