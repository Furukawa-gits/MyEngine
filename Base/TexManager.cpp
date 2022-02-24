#include"TexManager.h"

//������
void TexManager::Init(ID3D12Device* dev)
{
	HRESULT result = S_FALSE;

	descHeapdesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapdesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descHeapdesc.NumDescriptors = texSRVcount;
	result = dev->CreateDescriptorHeap(&descHeapdesc, IID_PPV_ARGS(&descHeap));
}

//�e�N�X�`���ǂݍ���
void TexManager::LoadTexture(UINT texnumber, const wchar_t* filename, ID3D12Device* dev)
{
	assert(texnumber <= texSRVcount - 1);

	HRESULT result;

	//WIC�e�N�X�`���̃��[�h
	TexMetadata metadata{};
	ScratchImage scratchImg{};

	result = LoadFromWICFile(
		filename,
		WIC_FLAGS_NONE,
		&metadata, scratchImg);

	const Image* img = scratchImg.GetImage(0, 0, 0);

	//�e�N�X�`���o�b�t�@���\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		metadata.format,
		metadata.width,
		(UINT)metadata.height,
		(UINT16)metadata.arraySize,
		(UINT16)metadata.mipLevels);

	//�e�N�X�`���o�b�t�@����
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&this->texBuff[texnumber]));

	//�e�N�X�`���o�b�t�@�ւ̃f�[�^�]��
	result = this->texBuff[texnumber]->WriteToSubresource(
		0,
		nullptr,
		img->pixels,
		(UINT)img->rowPitch,
		(UINT)img->slicePitch);

	//delete[] imageData;

	//�V�F�[�_�[���\�[�X�r���[�ݒ�
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	//�V�F�[�_�[���\�[�X�r���[�쐬
	dev->CreateShaderResourceView(this->texBuff[texnumber].Get(),
		&srvDesc,
		CD3DX12_CPU_DESCRIPTOR_HANDLE(this->descHeap->GetCPUDescriptorHandleForHeapStart(), texnumber,
			dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
		)
	);
}

//�V�F�[�_�[���\�[�X�r���[����
D3D12_GPU_DESCRIPTOR_HANDLE TexManager::FindReturnSRV(UINT texnumber, ID3D12Device* dev)
{
	return CD3DX12_GPU_DESCRIPTOR_HANDLE(
		descHeap->GetGPUDescriptorHandleForHeapStart(),
		texnumber,
		dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV));
}
