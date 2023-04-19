#pragma once
#include"../Base/TexManager.h"
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

//�p�C�v���C���Z�b�g
struct PipelineSet
{
	ComPtr<ID3D12PipelineState> pipelinestate;//�p�C�v���C���X�e�[�g

	ComPtr<ID3D12RootSignature> rootsignature;//���[�g�V�O�l�`��
};

class SpriteCommon
{
public:
	// �X�v���C�g�̋��ʃf�[�^
 
	//�p�C�v���C���Z�b�g
	PipelineSet pipelineset;

	//�ˉe�s��
	XMMATRIX matProjection{};

	//�p�C�v���C���ݒ�
	PipelineSet generatePipeLineStateSP(ID3D12Device* dev);

	//���ʃf�[�^����
	void spriteCommonCreate(ID3D12Device* dev , int window_width, int window_height);

	//�X�v���C�g���ʃO���t�B�b�N�X�R�}���h�̃Z�b�g
	void spriteCommonBeginDraw(ID3D12GraphicsCommandList* cmdlist, TexManager* tex);
};