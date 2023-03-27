#include"Light.h"

using namespace DirectX;

ID3D12Device* Light::device = nullptr;

void Light::Staticinitialize(ID3D12Device* device)
{
	//再初期化チェック
	assert(!Light::device);
	//nullptrチェック
	assert(device);
	//静的メンバ変数のセット
	Light::device = device;
}

void Light::TransferConstBuffer()
{
	HRESULT result;
	//定数バッファへデータ転送
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
	//定数バッファの生成
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
	//3dオブジェクトのインスタンスを生成
	Light* instance = new Light();
	//初期化
	instance->init();
	//生成したインスタンスを返す
	return instance;
}

