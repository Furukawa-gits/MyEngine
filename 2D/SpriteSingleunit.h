#pragma once
#include"SpriteCommon.h"
#include<DirectXTex.h>
#include<d3dx12.h>
#include<wrl.h>
#include<DirectXMath.h>
#include <d3dcompiler.h>
#include"../Base/WindowGenerate.h"
#include"../Base/TexManager.h"

#pragma comment(lib, "d3dcompiler.lib")

using XMFLOAT3 = DirectX::XMFLOAT3;

//�萔�o�b�t�@�p�f�[�^�\����
struct ConstBufferDataSP {
	XMFLOAT4 color;				//�F�@RGBA
	XMMATRIX mat;
};

//�X�v���C�g�p�O���[�o���ϐ�
struct VertexPosUv										//���_�\����(�X�v���C�g�p)
{
	XMFLOAT3 pos;
	XMFLOAT2 uv;
};

class SingleSprite
{
public:
	//�X�v���C�g�P�̒��_�o�b�t�@�̓]��
	void SpriteTransferVertexBuffer(TexManager* tex, bool isCutout);

	//�X�v���C�g����
	void GenerateSprite(ID3D12Device* dev,
		float window_width,
		float window_heigh,
		UINT texnumber,
		TexManager* tex,
		bool sizeFlag = false,
		bool isFlipX = false,
		bool isFlipY = false,
		bool iscutout = false);

	//�X�v���C�g�X�V����
	void SpriteUpdate(const SpriteCommon& spritecommon);

	//�X�v���C�g�`��R�}���h
	void DrawSprite(ID3D12GraphicsCommandList* cmdList, TexManager* texture, ID3D12Device* dev);

	//���_�o�b�t�@
	ComPtr<ID3D12Resource> spriteVertBuff;

	//���_�o�b�t�r���[
	D3D12_VERTEX_BUFFER_VIEW spriteVBView{};

	//�萔�o�b�t�@
	ComPtr<ID3D12Resource> spriteConstBuff;

	//Z������̉�]�p
	float rotation = 0.0f;

	//���W
	XMFLOAT3 position = { 0,0,0 };

	//���[���h�s��
	XMMATRIX matWorld;

	//�F(RGBA)
	XMFLOAT4 color = { 1,1,1,1 };

	//�e�N�X�`���ԍ�
	UINT texnumber = 0;

	//�傫��
	XMFLOAT2 size = { 100.0f,100.0f };

	//�A���J�[�|�C���g
	XMFLOAT2 anchorpoint = { 0.0f,0.0f };

	//���E���]
	bool isFlipX = false;

	//�㉺���]
	bool isFlipY = false;

	//�e�N�X�`��������W
	XMFLOAT2 texLeftTop = { 0,0 };

	//�e�N�X�`���؂�o���T�C�Y
	XMFLOAT2 texSize = { 100,100 };

	//��\��
	bool isInvisible = false;

public:

	static ComPtr<ID3D12PipelineState> SpritePipelinestate;//�p�C�v���C���X�e�[�g

	static ComPtr<ID3D12RootSignature> SpriteRootsignature;//���[�g�V�O�l�`��

	//�p�C�v���C���ݒ�
	static void SetPipelineStagte(ID3D12Device* dev);
};