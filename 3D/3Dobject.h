#pragma once
#include"ObjectCommon.h"
#include"../camera/Camera.h"

//using namespace DirectX;

//�萔�o�b�t�@�p�f�[�^�\����
struct ConstBufferDataB0 {
	//XMFLOAT4 color;				//�F�@RGBA
	XMMATRIX mat;
};

struct ConstBufferDataB1
{
	XMFLOAT3 ambient;
	float pad1;
	XMFLOAT3 diffuse;
	float pad2;
	XMFLOAT3 specular;
	float alpha;
};

//�����ς݂̃o�b�t�@�[�r���[�ɍ��킹�đ����Ă���
enum indices_num
{
	cube,
	tri,
	plane
};

float returnScaler(XMFLOAT3 vec);

XMFLOAT3 normalized(XMFLOAT3 vec);

class object3D
{
public:

	//�萔�o�b�t�@
	ComPtr<ID3D12Resource> constbuffB0;		//B0
	ComPtr<ID3D12Resource> constbuffB1;		//B1

	//�萔�o�b�t�@�r���[�̃n���h��(CPU)
	D3D12_CPU_DESCRIPTOR_HANDLE cpuDescHandleCBV;

	//�萔�o�b�t�@�r���[�̃n���h��(GPU)
	D3D12_GPU_DESCRIPTOR_HANDLE gpuDescHandleCBV;

	//�A�t�B���ϊ����
	XMFLOAT3 scale = { 1,1,1 };
	XMFLOAT3 rotation = { 0,0,0 };
	XMFLOAT3 position = { 0,0,0 };

	//���[���h�ϊ��s��
	XMMATRIX matWorld;

	//�e�I�u�W�F�N�g�ւ̃|�C���^
	object3D* parent = nullptr;

	//�t���O
	bool flag;

	//�R���X�g���N�^
	object3D();

	//�ÓI������
	static void StaticInit(Camera* camera = nullptr, directX* directx = nullptr);

	//�ÓI�f�[�^�Z�b�g
	static void SetStaticData(Camera* camera, directX* directx, ObjectCommon* common);

	//3D�I�u�W�F�N�g������
	void Init3d(int index);

	//3D�I�u�W�F�N�g�X�V����
	void Update3d(Viewes& view, XMFLOAT4 color = { 1,1,1,1 });

	//3D�I�u�W�F�N�g�`�揈��
	void Draw3d(ID3D12DescriptorHeap* TexdescHeap, Viewes& view, D3D12_GPU_DESCRIPTOR_HANDLE gpuDeschandleSRV, indices_num num);

	//���f����p�`�揈��(�}�e���A������)
	void DrawModel(ID3D12DescriptorHeap* TexdescHeap, Viewes& view, D3D12_GPU_DESCRIPTOR_HANDLE gpuDeschandleSRV);

	//���f���`��(�}�e���A������)
	void DrawModel_OnMaterial(Viewes& view);

	//���[���h���W����X�N���[�����W
	XMVECTOR WorldToScreenPos(XMVECTOR World_pos);

public:
	//�J����
	static Camera* camera;

	//directx��Ճf�[�^
	static directX* directx;

	//���ʃf�[�^
	static ObjectCommon* objectcommon;
};