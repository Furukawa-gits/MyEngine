#include"Light.h"

using namespace DirectX;

ID3D12Device* Light::device = nullptr;

void Light::Staticinitialize(ID3D12Device* device)
{
	//�ď������`�F�b�N
	assert(!Light::device);
	//nullptr�`�F�b�N
	assert(device);
	//�ÓI�����o�ϐ��̃Z�b�g
	Light::device = device;
}

void Light::TransferConstBuffer()
{
	HRESULT result;
	//�萔�o�b�t�@�փf�[�^�]��
	ConstBufferData* constMap = nullptr;
	result = constBuff->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result))
	{
		constMap->lightv = -lightdir;
		constMap->lightcolor = lightcolor;
		constBuff->Unmap(0, nullptr);
	}
}

void Light::init()
{
	HRESULT result;
	//�萔�o�b�t�@�̐���
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff));

	if (FAILED(result)) { assert(0); }

	TransferConstBuffer();
}

void Light::SetLightDir(const XMVECTOR& lightdir)
{
	this->lightdir = XMVector3Normalize(lightdir);
	dirty = true;
}

void Light::SetLightColor(const XMFLOAT3& lightcolor)
{
	this->lightcolor = lightcolor;
	dirty = true;
}

void Light::Update()
{
	if (dirty)
	{
		TransferConstBuffer();
		dirty - false;
	}
}

void Light::Draw(ID3D12GraphicsCommandList* cmdlist, UINT rootParameterindex)
{
	cmdlist->SetGraphicsRootConstantBufferView(rootParameterindex,
		constBuff->GetGPUVirtualAddress());
}

Light* Light::Create()
{
	//3d�I�u�W�F�N�g�̃C���X�^���X�𐶐�
	Light* instance = new Light();
	//������
	instance->init();
	//���������C���X�^���X��Ԃ�
	return instance;
}

