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

	//���_�\����(3D�I�u�W�F�N�g�p)
	struct VertexPos
	{
		XMFLOAT3 pos;
	};

	//�萔�o�b�t�@
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

	//�ÓI�֐�
	static void setStaticData(directX* dx);
	static void CreateGraphicsPipeline();

	/// <summary>
	/// �J�����Z�b�g
	/// </summary>
	/// <param name="cmr">�J�����̃|�C���^�[</param>
	static void setCamera(Camera* cmr) { camera = cmr; }

	//�����o�֐�
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

	//�p�C�v���C���X�e�[�g
	static ComPtr<ID3D12RootSignature> rootsignature;
	static ComPtr<ID3D12PipelineState> pipelinestate;

	//�萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuff;
	//���_�o�b�t�@
	ComPtr<ID3D12Resource> vertBuff;
	//���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView{};

	//�s��
	XMMATRIX matStart = {};
	XMMATRIX matEnd = {};

	//2�_
	XMFLOAT3 startPoint = { 0,0,0 };
	XMFLOAT3 endPoint = { 0,0,0 };

	//�F
	XMFLOAT4 color = { 1,1,1,1 };
};