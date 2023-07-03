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

	//���_�\����(3D�I�u�W�F�N�g�p)
	struct Vertex
	{
		XMFLOAT3 pos;
	};

	//�萔�o�b�t�@
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

	//�ÓI�֐�
	static void CreateGraphicsPipeline();

	//�����o�֐�
	void init();

	void updata();

	void draw();

	void setPoints(XMFLOAT3 start, XMFLOAT3 end)
	{
		startPoint = start;
		endPoint = end;
	}

private:

	//�p�C�v���C���X�e�[�g
	static ComPtr<ID3D12RootSignature> rootsignature;
	static ComPtr<ID3D12PipelineState> pipelinestate;

	//�萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuff;

	//�F
	XMFLOAT4 color = { 1,1,1,1 };

	//2�_
	XMFLOAT3 startPoint = { 0,0,0 };
	XMFLOAT3 endPoint = { 0,0,0 };

	//�s��
	XMMATRIX matStart = {};
	XMMATRIX matEnd = {};

};