#include"SpriteSingleunit.h"

//�X�v���C�g�P�̒��_�o�b�t�@�̓]��
void SingleSprite::SpriteTransferVertexBuffer(TexManager* tex, bool isCutout)
{
	HRESULT result = S_FALSE;

	VertexPosUv vertices[] =
	{
		{{},{0.0f,1.0f}},
		{{},{0.0f,0.0f}},
		{{},{1.0f,1.0f}},
		{{},{1.0f,0.0f}}
	};

	enum { LB, LT, RB, RT };

	float left = (0.0f - this->anchorpoint.x) * this->size.x;
	float right = (1.0f - this->anchorpoint.x) * this->size.x;
	float top = (0.0f - this->anchorpoint.y) * this->size.y;
	float bottom = (1.0f - this->anchorpoint.y) * this->size.y;

	if (this->isFlipX == true)
	{
		left = -left;
		right = -right;
	}

	if (this->isFlipY == true)
	{
		top = -top;
		bottom = -bottom;
	}


	vertices[LB].pos = {  left,bottom,0.0f };
	vertices[LT].pos = {  left,   top,0.0f };
	vertices[RB].pos = { right,bottom,0.0f };
	vertices[RT].pos = { right,   top,0.0f };

	//�؂蔲������
	if (isCutout == true)
	{
		D3D12_RESOURCE_DESC resdesc = tex->texBuff[this->texnumber]->GetDesc();

		float tex_left = this->texLeftTop.x / resdesc.Width;
		float tex_right = (this->texLeftTop.x + this->texSize.x) / resdesc.Width;
		float tex_top = this->texLeftTop.y / resdesc.Height;
		float tex_bottom = (this->texLeftTop.y + this->texSize.y) / resdesc.Height;

		vertices[LB].uv = { tex_left,tex_bottom };
		vertices[LT].uv = { tex_left,tex_top };
		vertices[RB].uv = { tex_right,tex_bottom };
		vertices[RT].uv = { tex_right,tex_top };
	}

	//���_�o�b�t�@�ւ̃f�[�^�]��
	VertexPosUv* vertMap = nullptr;
	result = this->spriteVertBuff->Map(0, nullptr, (void**)&vertMap);
	memcpy(vertMap, vertices, sizeof(vertices));
	this->spriteVertBuff->Unmap(0, nullptr);
}

//�X�v���C�g����
void SingleSprite::GenerateSprite(ID3D12Device* dev, 
	float window_width, 
	float window_heigh, 
	UINT texnumber, 
	TexManager* tex,
	bool sizeFlag, 
	bool isFlipX, 
	bool isFlipY, 
	bool iscutout)
{
	HRESULT result = S_FALSE;

	//���_�f�[�^
	VertexPosUv vertices[] =
	{
		{{  0.0f,100.0f,0.0f},{0.0f,1.0f}},	//����
		{{  0.0f,  0.0f,0.0f},{0.0f,0.0f}},	//����
		{{100.0f,100.0f,0.0f},{1.0f,1.0f}},	//�E��
		{{100.0f,  0.0f,0.0f},{1.0f,0.0f}}	//�E��
	};

	this->texnumber = texnumber;

	//���_�o�b�t�@����
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(vertices)),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&spriteVertBuff));

	if (sizeFlag == true)
	{
		D3D12_RESOURCE_DESC resdesc = tex->texBuff[texnumber]->GetDesc();

		size = { (float)resdesc.Width,(float)resdesc.Height };
	}

	this->isFlipX = isFlipX;
	this->isFlipY = isFlipY;

	//�o�b�t�@�ւ̃f�[�^�]��
	SpriteTransferVertexBuffer(tex, iscutout);


	//���_�o�b�t�@�r���[�̍쐬
	this->spriteVBView.BufferLocation = this->spriteVertBuff->GetGPUVirtualAddress();
	this->spriteVBView.SizeInBytes = sizeof(vertices);
	this->spriteVBView.StrideInBytes = sizeof(vertices[0]);

	//�萔�o�b�t�@�̐���
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataSP) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&this->spriteConstBuff));

	//�萔�o�b�t�@�Ƀf�[�^�]��
	ConstBufferDataSP* constMap = nullptr;
	result = this->spriteConstBuff->Map(0, nullptr, (void**)&constMap);
	constMap->color = XMFLOAT4(1, 1, 1, 1);
	constMap->mat = XMMatrixOrthographicOffCenterLH(
		0.0f, win_width, win_hight, 0.0f, 0.0f, 1.0f);
	this->spriteConstBuff->Unmap(0, nullptr);
}

//�X�v���C�g�X�V����
void SingleSprite::SpriteUpdate(const SpriteCommon& spritecommon)
{
	matWorld = XMMatrixIdentity();

	matWorld *= XMMatrixRotationZ(XMConvertToRadians(rotation));

	matWorld *= XMMatrixTranslation(position.x, position.y, position.z);

	ConstBufferDataSP* constMap = nullptr;
	HRESULT result = spriteConstBuff->Map(0, nullptr, (void**)&constMap);
	constMap->color = color;
	constMap->mat = matWorld * spritecommon.matProjection;
	spriteConstBuff->Unmap(0, nullptr);
}

//�X�v���C�g�`��R�}���h
void SingleSprite::DrawSprite(ID3D12GraphicsCommandList* cmdList, TexManager* texture, ID3D12Device* dev)
{
	if (isInvisible == true)
	{
		return;
	}
	//���_�o�b�t�@���Z�b�g
	cmdList->IASetVertexBuffers(0, 1, &spriteVBView);

	//�萔�o�b�t�@���Z�b�g
	cmdList->SetGraphicsRootConstantBufferView(0, spriteConstBuff->GetGPUVirtualAddress());

	//�V�F�[�_�[���\�[�X�r���[���Z�b�g
	cmdList->SetGraphicsRootDescriptorTable(1,
		texture->FindReturnSRV(texnumber, dev));

	//�`��R�}���h
	cmdList->DrawInstanced(4, 1, 0, 0);
}
