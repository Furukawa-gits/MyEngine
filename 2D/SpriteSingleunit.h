#pragma once
#include"SpriteCommon.h"
#include<DirectXTex.h>
#include<d3dx12.h>
#include<wrl.h>
#include<string>
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

//���_�\����(�X�v���C�g�p)
struct VertexPosUv
{
	XMFLOAT3 pos;
	XMFLOAT2 uv;
};

class SingleSprite
{
public:
	/// <summary>
	/// �p�C�v���C������
	/// </summary>
	static void SetPipelineStateSprite();

	/// <summary>
	/// ���_�o�b�t�@�ւ̃f�[�^�]��
	/// </summary>
	/// <param name="tex">�e�N�X�`���}�l�[�W���[</param>
	/// <param name="isCutout">�؂��邩�ǂ���</param>
	void SpriteTransferVertexBuffer(TexManager* tex, bool isCutout);

	/// <summary>
	/// �e�N�X�`���ǂݍ���
	/// </summary>
	/// <param name="filename">�t�@�C����</param>
	void LoadTexture(const std::string& filename);

	/// <summary>
	/// �X�v���C�g����
	/// </summary>
	/// <param name="dev">�f�o�C�X���</param>
	/// <param name="texnumber">�e�N�X�`���ԍ�</param>
	/// <param name="tex">�e�N�X�`���}�l�[�W���[</param>
	/// <param name="sizeFlag">�摜�T�C�Y�ɐ؂蔲��</param>
	/// <param name="isFlipX">���E���]</param>
	/// <param name="isFlipY">�㉺���]</param>
	/// <param name="iscutout">�؂蔲�����ǂ���</param>
	void GenerateSprite(ID3D12Device* dev,
		UINT texnumber,
		TexManager* tex,
		bool sizeFlag = false,
		bool isFlipX = false,
		bool isFlipY = false,
		bool iscutout = false);

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="spritecommon">�X�v���C�g���ʃf�[�^</param>
	void SpriteUpdate(const SpriteCommon& spritecommon);

	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="cmdList">�R�}���h���X�g</param>
	/// <param name="texture">�e�N�X�`���}�l�[�W���[</param>
	void DrawSprite(ID3D12GraphicsCommandList* cmdList, TexManager* texture);

public:
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

private:
	static ID3D12Device* device;

	static ComPtr<ID3D12RootSignature> SpriteRootsignature;//���[�g�V�O�l�`��

	static ComPtr<ID3D12PipelineState> SpritePipelinestate;//�p�C�v���C���X�e�[�g

	//���_�o�b�t�@
	ComPtr<ID3D12Resource> spriteVertBuff;

	//���_�o�b�t�r���[
	D3D12_VERTEX_BUFFER_VIEW spriteVBView{};

	//�萔�o�b�t�@
	ComPtr<ID3D12Resource> spriteConstBuff;

	//�e�N�X�`���o�b�t�@
	ComPtr<ID3D12Resource> texbuff;

	//�V�F�[�_�[���\�[�X�r���[
	ComPtr<ID3D12DescriptorHeap> descHeapSRV;
};