#pragma once
#include"SpriteSingleunit.h"
#include"../Base/DirectX_Base.h"

class PostEffect
{
public:
	//�R���X�g���N�^
	PostEffect();

	//������
	void Init(ID3D12Device* dev);

	//�`��
	void Draw(ID3D12GraphicsCommandList* cmdList, ID3D12Device* dev);

	//�`��O����
	void PreDrawScene(ID3D12GraphicsCommandList* cmdlist, ID3D12Device* dev);

	//�`��㏈��
	void PostDrawScene(ID3D12GraphicsCommandList* cmdlist, directX* directx);

	//�[�x�o�b�t�@�N���A
	void depthClear(ID3D12GraphicsCommandList* cmdlist);

	//�p�C�v���C������
	void CreateGraphicsPipelineState(ID3D12Device* dev);

	//�ʏ�
	ComPtr<ID3D12PipelineState> PostPipelinestate;//�p�C�v���C���X�e�[�g
	ComPtr<ID3D12RootSignature> PostRootsignature;//���[�g�V�O�l�`��

	ComPtr<ID3D12Resource> texbuff[2];

	ComPtr<ID3D12DescriptorHeap> descHeapSRV;

	ComPtr<ID3D12Resource> depthbuff;

	ComPtr<ID3D12DescriptorHeap> descHeapRTV;

	ComPtr<ID3D12DescriptorHeap> descheapDSV;

	D3D12_CPU_DESCRIPTOR_HANDLE dsvH;

	//���_�o�b�t�@
	ComPtr<ID3D12Resource> PostVertBuff;

	//���_�o�b�t�r���[
	D3D12_VERTEX_BUFFER_VIEW PostVBView{};

	//�萔�o�b�t�@
	ComPtr<ID3D12Resource> PostConstBuff;

	static const float clearColor[4];
};

