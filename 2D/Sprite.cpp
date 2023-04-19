#include"Sprite.h"

ID3D12Device* SingleSprite::device = nullptr;
dxinput* SingleSprite::input = nullptr;
ComPtr<ID3D12RootSignature> SingleSprite::spriteRootsignature;
ComPtr<ID3D12PipelineState> SingleSprite::spritePipelinestate;
XMMATRIX SingleSprite::matprojection;

void SingleSprite::setStaticData(ID3D12Device* dev, dxinput* dxinput)
{
	device = dev;

	input = dxinput;

	//�p�C�v���C������
	setPipelineStateSprite();

	matprojection = XMMatrixOrthographicOffCenterLH(
		0.0f, (float)windowWidth, (float)windowHight, 0.0f, 0.0f, 1.0f);
}

//�O���t�B�b�N�X�p�C�v���C������
void SingleSprite::setPipelineStateSprite()
{
	if (spritePipelinestate != NULL && spriteRootsignature != NULL)
	{
		return;
	}

	HRESULT result;

	ComPtr<ID3DBlob> vsBlob = nullptr; // ���_�V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> psBlob = nullptr; // �s�N�Z���V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> errorBlob = nullptr; // �G���[�I�u�W�F�N�g

	// ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/shaders/SpriteVS.hlsl",  // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "vs_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&vsBlob, &errorBlob);

	// �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/shaders/SpritePS.hlsl",   // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "ps_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&psBlob, &errorBlob);

	if (FAILED(result)) {
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	//���_�V�F�[�_�[�ɓn�����߂̒��_�f�[�^�𐮂���
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},

		{
			"TEXCORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		}
	};

	// �O���t�B�b�N�X�p�C�v���C���X�e�[�g�ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};

	//���_�V�F�[�_�A�s�N�Z���V�F�[�_���p�C�v���C���ɐݒ�
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

	//�T���v���}�X�N�ƃ��X�^���C�U�X�e�[�g�̐ݒ�
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; //�W���ݒ�
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

	//�u�����h�X�e�[�g�̐ݒ�
	D3D12_RENDER_TARGET_BLEND_DESC& blenddsec = gpipeline.BlendState.RenderTarget[0];
	blenddsec.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	//�u�����h���ʐݒ�
	blenddsec.BlendEnable = true;
	blenddsec.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddsec.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddsec.DestBlendAlpha = D3D12_BLEND_ZERO;

	//�����ݒ�
	blenddsec.BlendOp = D3D12_BLEND_OP_ADD;
	blenddsec.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddsec.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	//���_���C�A�E�g�̐ݒ�
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	//�}�`�̌`����O�p�`�ɐݒ�
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	//���̑��̐ݒ�
	gpipeline.NumRenderTargets = 1; //�`��Ώۂ�1��
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; //0�`255�w���RGBA
	gpipeline.SampleDesc.Count = 1; //1�s�N�Z���ɂ�1��T���v�����O

	//�f�v�X�X�e���V���X�e�[�g�̐ݒ�
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	gpipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;

	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	//�f�X�N���v�^�����W�̐ݒ�
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;

	//�f�X�N���v�^�e�[�u���̐ݒ�
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

	//���[�g�p�����[�^�̐ݒ�
	CD3DX12_ROOT_PARAMETER rootparams[2];
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);//�萔�o�b�t�@�r���[�Ƃ��ď�����
	rootparams[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);

	//�e�N�X�`���T���v���[�̐ݒ�
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc{};
	samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);

	//���[�g�V�O�l�`���̐���
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob;
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc,
		D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(),
		rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&spriteRootsignature));

	//�p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
	gpipeline.pRootSignature = spriteRootsignature.Get();

	//�p�C�v���C���X�e�[�g�̐���
	result = device->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&spritePipelinestate));
}

//�X�v���C�g�P�̒��_�o�b�t�@�̓]��
void SingleSprite::spriteTransferVertexBuffer(bool isCutout)
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


	vertices[LB].pos = { left,bottom,0.0f };
	vertices[LT].pos = { left,   top,0.0f };
	vertices[RB].pos = { right,bottom,0.0f };
	vertices[RT].pos = { right,   top,0.0f };

	//�؂蔲������
	if (isCutout == true)
	{
		D3D12_RESOURCE_DESC resdesc = texbuff->GetDesc();

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

//�e�N�X�`���ǂݍ���
void SingleSprite::loadTexture(const std::string& filename)
{
	HRESULT result;

	const std::string baseDirectory = "Resources/Image/";

	std::string fullpath = baseDirectory + filename;

	wchar_t wfullpath[128];
	MultiByteToWideChar(CP_ACP, 0, fullpath.c_str(), -1, wfullpath, _countof(wfullpath));

	//WIC�e�N�X�`���̃��[�h
	TexMetadata metadata{};
	ScratchImage scratchImg{};

	result = LoadFromWICFile(
		wfullpath,
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

	D3D12_DESCRIPTOR_HEAP_DESC descHeapdesc = {};
	descHeapdesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapdesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descHeapdesc.NumDescriptors = 1;
	result = device->CreateDescriptorHeap(&descHeapdesc, IID_PPV_ARGS(&descHeapSRV));

	//�e�N�X�`���o�b�t�@����
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&this->texbuff));

	//�e�N�X�`���o�b�t�@�ւ̃f�[�^�]��
	result = this->texbuff->WriteToSubresource(
		0,
		nullptr,
		img->pixels,
		(UINT)img->rowPitch,
		(UINT)img->slicePitch);

	//�V�F�[�_�[���\�[�X�r���[�ݒ�
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	//�V�F�[�_�[���\�[�X�r���[�쐬
	device->CreateShaderResourceView(this->texbuff.Get(),
		&srvDesc,
		CD3DX12_CPU_DESCRIPTOR_HANDLE(this->descHeapSRV->GetCPUDescriptorHandleForHeapStart(), 0,
			device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
		)
	);
}

//�X�v���C�g����
void SingleSprite::generateSprite(
	const std::string& filename,
	bool sizeFlag,
	bool isFlipX,
	bool isFlipY,
	bool iscutout)
{
	loadTexture(filename);

	HRESULT result = S_FALSE;

	//���_�f�[�^
	VertexPosUv vertices[] =
	{
		{{  0.0f,100.0f,0.0f},{0.0f,1.0f}},	//����
		{{  0.0f,  0.0f,0.0f},{0.0f,0.0f}},	//����
		{{100.0f,100.0f,0.0f},{1.0f,1.0f}},	//�E��
		{{100.0f,  0.0f,0.0f},{1.0f,0.0f}}	//�E��
	};

	//���_�o�b�t�@����
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(vertices)),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&spriteVertBuff));

	if (sizeFlag == true)
	{
		D3D12_RESOURCE_DESC resdesc = texbuff->GetDesc();

		size = { (float)resdesc.Width,(float)resdesc.Height };
	}

	this->isFlipX = isFlipX;
	this->isFlipY = isFlipY;

	//�o�b�t�@�ւ̃f�[�^�]��
	spriteTransferVertexBuffer(iscutout);

	//���_�o�b�t�@�r���[�̍쐬
	this->spriteVBView.BufferLocation = this->spriteVertBuff->GetGPUVirtualAddress();
	this->spriteVBView.SizeInBytes = sizeof(vertices);
	this->spriteVBView.StrideInBytes = sizeof(vertices[0]);

	//�萔�o�b�t�@�̐���
	result = device->CreateCommittedResource(
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
		0.0f, windowWidth, windowHight, 0.0f, 0.0f, 1.0f);
	this->spriteConstBuff->Unmap(0, nullptr);

	//���͗p�̃T�C�Y��ݒ�
	setBaceSize(this->size);
}

//�X�v���C�g�X�V����
void SingleSprite::spriteUpdata(bool iscutout)
{
	matWorld = XMMatrixIdentity();

	matWorld *= XMMatrixRotationZ(XMConvertToRadians(rotation));

	matWorld *= XMMatrixTranslation(position.x, position.y, position.z);

	spriteTransferVertexBuffer(iscutout);

	ConstBufferDataSP* constMap = nullptr;
	HRESULT result = spriteConstBuff->Map(0, nullptr, (void**)&constMap);
	constMap->color = color;
	constMap->mat = matWorld * matprojection;
	spriteConstBuff->Unmap(0, nullptr);
}

//�X�v���C�g�`��R�}���h
void SingleSprite::drawSprite(ID3D12GraphicsCommandList* cmdList)
{
	cmdList->SetPipelineState(spritePipelinestate.Get());

	cmdList->SetGraphicsRootSignature(spriteRootsignature.Get());

	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	ID3D12DescriptorHeap* ppHeaps[] = { descHeapSRV.Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	if (isInvisible == true)
	{
		return;
	}
	//���_�o�b�t�@���Z�b�g
	cmdList->IASetVertexBuffers(0, 1, &spriteVBView);

	//�萔�o�b�t�@���Z�b�g
	cmdList->SetGraphicsRootConstantBufferView(0, spriteConstBuff->GetGPUVirtualAddress());

	//�V�F�[�_�[���\�[�X�r���[���Z�b�g
	cmdList->SetGraphicsRootDescriptorTable(1, descHeapSRV->GetGPUDescriptorHandleForHeapStart());

	//�`��R�}���h
	cmdList->DrawInstanced(4, 1, 0, 0);
}

//�}�E�X����
bool SingleSprite::isSpriteMouseInput()
{
	float left = (0.0f - this->anchorpoint.x) * this->baceSize.x + position.x;
	float right = (1.0f - this->anchorpoint.x) * this->baceSize.x + position.x;
	float top = (0.0f - this->anchorpoint.y) * this->baceSize.y + position.y;
	float bottom = (1.0f - this->anchorpoint.y) * this->baceSize.y + position.y;

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

	this->size = this->baceSize;

	isMouseSelect = false;

	if (left < input->mousePosition.x && input->mousePosition.x < right)
	{
		if (top < input->mousePosition.y && input->mousePosition.y < bottom)
		{
			this->size = this->choosingSize;

			isMouseSelect = true;

			if (input->Mouse_LeftTriger())
			{
				return true;
			}
		}
	}

	return false;
}