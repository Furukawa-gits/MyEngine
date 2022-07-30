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

//�萔�o�b�t�@�p�f�[�^�\����(��)
struct ConstBufferDataL {
	XMFLOAT4 color = { 1,1,1,1 };
	XMMATRIX startMat;
	XMMATRIX endMat;
};

//���_�\����(��)
struct VertexPos
{
	XMFLOAT3 Pos;
};

//��
class primitiveLine2D
{
public:
	static void SetStaticData(ID3D12Device* dev);
	void lineTransferVertexBuffer();
	void init();
	void update();
	void draw(ID3D12GraphicsCommandList* cmdList);

private:
	static void createGP();

	static ID3D12Device* device;

	static ComPtr<ID3D12RootSignature> lineRootsignature;//���[�g�V�O�l�`��

	static ComPtr<ID3D12PipelineState> linePipelinestate;//�p�C�v���C���X�e�[�g

	static XMMATRIX matprojection;

	//���_�o�b�t�@
	ComPtr<ID3D12Resource> lineVertBuff;
	//�萔�o�b�t�@
	ComPtr<ID3D12Resource> lineConstBuff;
	//���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView{};

	//���[���h�s��
	XMMATRIX startMatWorld;
	XMMATRIX endMatWorld;

public:
	XMFLOAT3 startPos = { 0,0,0 };
	XMFLOAT3 endPos = { 0,0,0 };

	XMFLOAT4 color = { 1,1,1,1 };
};