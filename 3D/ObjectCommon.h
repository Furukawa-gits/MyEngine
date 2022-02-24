#pragma once
#include"../Base/DirectX_Base.h"
#include"../Base/TexManager.h"
#include<DirectXMath.h>
#include<cmath>
#include<string>
#include <DirectXTex.h>
#include <cassert>
#include<vector>
#include<unordered_map>
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;
using namespace std;

//�p�C�v���C���Z�b�g
struct PipelineSet3D
{
	ComPtr<ID3D12PipelineState> pipelinestate;				//�p�C�v���C���X�e�[�g

	ComPtr<ID3D12RootSignature> rootsignature;				//���[�g�V�O�l�`��
};

//���_�\����(3D�I�u�W�F�N�g�p)
struct Vertex
{
	XMFLOAT3 pos;
	XMFLOAT3 normal;
	XMFLOAT2 uv;
};

//�}�e���A���\����
struct Material
{
	std::string name;
	XMFLOAT3 ambient;
	XMFLOAT3 diffuse;
	XMFLOAT3 specular;
	float alpha;
	std::string textureFilename;
	//�R���X�g���N�^
	Material()
	{
		ambient = { 0.3f,0.3f,0.3f };
		diffuse = { 0.0f,0.0f,0.0f };
		specular = { 0.0f,0.0f,0.0f };
		alpha = 1.0f;
	}

	// �V�F�[�_���\�[�X�r���[�̃n���h��(CPU)
	CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
	// �V�F�[�_���\�[�X�r���[�̃n���h��(CPU)
	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;

	int material_texnumber;
};

//�o�b�t�@�r���[�\����
struct Viewes
{
	D3D12_VERTEX_BUFFER_VIEW vbView{};						//���_�o�b�t�@�r���[
	D3D12_INDEX_BUFFER_VIEW idView{};						//�C���f�b�N�X�o�b�t�@�r���[
	//���_�o�b�t�@�̐���
	ComPtr<ID3D12Resource> vertBuff;
	ComPtr<ID3D12Resource> indexBuff;
	//���_�C���f�b�N�X�z��_�ۑ��p
	unsigned short Cubeindices[36];
	unsigned short Triindices[18];
	unsigned short Planeindices[6];

	//���f���p���_�z��
	std::vector<Vertex> vertices;

	//���f���p�C���f�b�N�X�z��
	std::vector<unsigned short> modelindices;

	//�}�e���A�����
	Material material;
};


//3d�I�u�W�F�N�g���ʃN���X
class ObjectCommon
{
public:
	//�萔�o�b�t�@�̍ő吔
	const int constantBufferNum = 2048;

	//�萔�o�b�t�@�p�̃f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> basicDescHeap;

	//�o�b�t�@�r���[�擾_������
	Viewes ReturnCubeViewes();

	//�o�b�t�@�r���[�擾_�O�p��
	Viewes ReturnTripyramidViewes();

	//�o�b�t�@�r���[�擾_��
	Viewes ReturnPlaneViewes();

	//�}�e���A���p�̃e�N�X�`���ǂݍ���
	void Material_Loadtexture(Material* material, const std::string& filename);

	//�}�e���A���ǂݍ���
	void LoadMaterial(Material* material, const std::string& filename);

	//�o�b�t�@�r���[�擾_���f��
	Viewes ReturnModelViewes(const char* Filename,int material_texnum);

	//�p�C�v���C��
	PipelineSet3D pipelineset;

	//�p�C�v���C���ݒ�
	PipelineSet3D  GeneratePipeLineState(ID3D12Device* dev);

	//������
	void init(directX* directx);

	//3D�I�u�W�F�N�g���ʕ`�揈��
	void object3DcommonBeginDraw();

public:
	directX* directx;

	UINT descriptorHandleIncrementSize;

	ComPtr<ID3D12DescriptorHeap> Material_descHeap;

	ComPtr<ID3D12Resource> texbuff[512];
};