#pragma once
#include"Sprite.h"
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

	void setPipelineGray(ID3D12GraphicsCommandList* cmdList);
	void setPipelineGaussian(ID3D12GraphicsCommandList* cmdList);

	void reSetPipeline();

	//�`��O����
	void preDrawScene(ID3D12GraphicsCommandList* cmdlist, ID3D12Device* dev);

	//�`��㏈��
	void postDrawScene(ID3D12GraphicsCommandList* cmdlist, directX* directx);

	//�[�x�o�b�t�@�N���A
	void depthClear(ID3D12GraphicsCommandList* cmdlist);

private:

	//�p�C�v���C������
	void createGraphicsPipelineState(ID3D12Device* dev);//�ʏ�
	void createGraphicsPipelineStateGray(ID3D12Device* dev);//�O���[�X�P�[��
	void createGraphicsPipelineStateGaussian(ID3D12Device* dev);//�K�E�V�A���u���[

	//�ʏ�
	ComPtr<ID3D12PipelineState> postPipelinestateBace;//�p�C�v���C���X�e�[�g
	ComPtr<ID3D12RootSignature> postRootsignatureBace;//���[�g�V�O�l�`��

	ComPtr<ID3D12PipelineState> postPipelinestateGray;//�p�C�v���C���X�e�[�g
	ComPtr<ID3D12RootSignature> postRootsignatureGray;//���[�g�V�O�l�`��

	ComPtr<ID3D12PipelineState> postPipelinestateGaussian;//�p�C�v���C���X�e�[�g
	ComPtr<ID3D12RootSignature> postRootsignatureGaussian;//���[�g�V�O�l�`��

	ComPtr<ID3D12Resource> texbuff[2];

	ComPtr<ID3D12DescriptorHeap> descHeapSRV;

	ComPtr<ID3D12Resource> depthbuff;

	ComPtr<ID3D12DescriptorHeap> descHeapRTV;

	ComPtr<ID3D12DescriptorHeap> descheapDSV;

	D3D12_CPU_DESCRIPTOR_HANDLE dsvH;

	//���_�o�b�t�@
	ComPtr<ID3D12Resource> postVertBuff;

	//���_�o�b�t�r���[
	D3D12_VERTEX_BUFFER_VIEW postVBView{};

	//�萔�o�b�t�@
	ComPtr<ID3D12Resource> postConstBuff;

	//�N���A�J���[
	static const float clearColor[4];

	//�ق��̃p�C�v���C�����Z�b�g���Ă��邩�ǂ���
	bool isSetOtherPipeline = false;
};

