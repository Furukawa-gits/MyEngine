#pragma once
#include"../Base/Window_Generate.h"
#include"SpriteCommon.h"
#include"../Base/TexManager.h"
#include<DirectXMath.h>

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
	XMFLOAT2 anchorpoint = { 0.5f,0.5f };

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

	//�X�v���C�g�P�̒��_�o�b�t�@�̓]��
	void SpriteTransferVertexBuffer(TexManager* tex, bool isCutout);

	//�X�v���C�g����
	void GenerateSprite(ID3D12Device* dev,
		float window_width,
		float window_heigh,
		UINT texnumber,
		TexManager* tex,
		bool sizeFlag,
		bool isFlipX,
		bool isFlipY,
		bool iscutout,
		XMFLOAT2 anchorpoint = { 0.5f,0.5f });

	//�X�v���C�g�X�V����
	void SpriteUpdate(const SpriteCommon& spritecommon);

	//�X�v���C�g�`��R�}���h
	void DrawSprite(ID3D12GraphicsCommandList* cmdList, TexManager* texture, ID3D12Device* dev);
};