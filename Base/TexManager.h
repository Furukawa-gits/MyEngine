#pragma once
#include<DirectXTex.h>
#include<d3dx12.h>
#include<wrl.h>

#pragma comment(lib,"d3d12.lib")

using namespace Microsoft::WRL;

using namespace DirectX;

const int texSRVcount = 512;

class TexManager
{
public:
	//�e�N�X�`���p�f�X�N���v�^�q�[�v�̐���
	ComPtr<ID3D12DescriptorHeap> descHeap;

	D3D12_DESCRIPTOR_HEAP_DESC descHeapdesc{};

	//�e�N�X�`�����\�[�X
	ComPtr<ID3D12Resource> texBuff[texSRVcount];

	//������
	void Init(ID3D12Device* dev);

	//�e�N�X�`���̓ǂݍ���
	void LoadTexture(UINT texnumber, const wchar_t* filename, ID3D12Device* dev);

	//�e�N�X�`���ԍ��ŃV�F�[�_�[���\�[�X�r���[���������ĕԂ���
	D3D12_GPU_DESCRIPTOR_HANDLE FindReturnSRV(UINT texnumber, ID3D12Device* dev);
};