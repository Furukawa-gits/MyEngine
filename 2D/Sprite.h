#pragma once
#include"../Base/WindowGenerate.h"
#include"../Input/dxInput.h"
#include<DirectXTex.h>
#include<d3dx12.h>
#include<wrl.h>
#include<string>
#include<DirectXMath.h>
#include <d3dcompiler.h>

#pragma comment(lib, "d3dcompiler.lib")

using namespace Microsoft::WRL;
using namespace DirectX;

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
private:
	/// <summary>
	/// �p�C�v���C������
	/// </summary>
	static void setPipelineStateSprite();

	/// <summary>
	/// �e�N�X�`���ǂݍ���
	/// xxxxx.png�̂�
	/// </summary>
	/// <param name="filename">�t�@�C����</param>
	void loadTexture(const std::string& filename);

public:
	/// <summary>
	/// �ÓI�f�[�^�Z�b�g
	/// </summary>
	/// <param name="dev">�f�o�C�X���</param>
	static void setStaticData(ID3D12Device* dev, dxinput* dxinput);

	/// <summary>
	/// ���_�o�b�t�@�ւ̃f�[�^�]��
	/// </summary>
	/// <param name="tex">�e�N�X�`���}�l�[�W���[</param>
	/// <param name="isCutout">�؂��邩�ǂ���</param>
	void spriteTransferVertexBuffer(bool isCutout = false);

	/// <summary>
	/// �X�v���C�g����
	/// </summary>
	/// <param name="filename">�t�@�C����(xxxxx.png�̂�)</param>
	/// <param name="sizeFlag">�摜�T�C�Y�ɐ؂蔲��</param>
	/// <param name="isFlipX">���E���]</param>
	/// <param name="isFlipY">�㉺���]</param>
	/// <param name="iscutout">�؂蔲�����ǂ���</param>
	void generateSprite(
		const std::string& filename,
		bool sizeFlag = false,
		bool isFlipX = false,
		bool isFlipY = false,
		bool iscutout = false);

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="spritecommon">�X�v���C�g���ʃf�[�^</param>
	void spriteUpdata();

	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="cmdList">�R�}���h���X�g</param>
	/// <param name="texture">�e�N�X�`���}�l�[�W���[</param>
	void drawSprite(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// �X�v���C�g�̃}�E�X����
	/// </summary>
	bool isSpriteMouseInput();

	/// <summary>
	/// ��̃T�C�Y�����
	/// </summary>
	/// <param name="bacesize">��T�C�Y</param> 
	void setBaceSize(XMFLOAT2 bacesize)
	{
		baceSize = bacesize;
		choosingSize = { bacesize.x * 1.3f,bacesize.y * 1.3f };
	}

public:
	//Z������̉�]�p
	float rotation = 0.0f;

	//���W
	XMFLOAT3 position = { 0,0,0 };

	//���[���h�s��
	XMMATRIX matWorld;

	//�F(RGBA)
	XMFLOAT4 color = { 1,1,1,1 };

	//�傫��
	XMFLOAT2 size = { 100.0f,100.0f };

	//���͗p�T�C�Y
	//��̑傫��
	XMFLOAT2 baceSize;

	//�I�𒆂̑傫��
	XMFLOAT2 choosingSize;

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

	//�}�E�X���W���d�Ȃ��Ă��邩
	bool isMouseSelect = false;

private:
	static ID3D12Device* device;

	static dxinput* input;

	static ComPtr<ID3D12RootSignature> spriteRootsignature;//���[�g�V�O�l�`��

	static ComPtr<ID3D12PipelineState> spritePipelinestate;//�p�C�v���C���X�e�[�g

	static XMMATRIX matprojection;

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