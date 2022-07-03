#include "PostEffect.h"
#include"../Base/WindowGenerate.h"
#include <d3dcompiler.h>

#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;

const float PostEffect::clearColor[4] = { 0.25f,0.5f ,0.1f ,0.0f };

PostEffect::PostEffect()
{
}

void PostEffect::Init(ID3D12Device* dev)
{
	HRESULT result;

	//���_�o�b�t�@����
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(VertexPosUv) * 4),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&PostVertBuff));

	//���_�f�[�^
	VertexPosUv vertices[4] =
	{
		{{-0.5f,-0.5f,0.0f},{0.0f,1.0f}},	//����
		{{-0.5f,+0.5f,0.0f},{0.0f,0.0f}},	//����
		{{+0.5f,-0.5f,0.0f},{1.0f,1.0f}},	//�E��
		{{+0.5f,+0.5f,0.0f},{1.0f,0.0f}}	//�E��
	};

	//���_�o�b�t�@�ւ̃f�[�^�]��
	VertexPosUv* vertMap = nullptr;
	result = this->PostVertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result)) {
		memcpy(vertMap, vertices, sizeof(vertices));
		this->PostVertBuff->Unmap(0, nullptr);
	}

	//���_�o�b�t�@�r���[�̍쐬
	this->PostVBView.BufferLocation = this->PostVertBuff->GetGPUVirtualAddress();
	this->PostVBView.SizeInBytes = sizeof(VertexPosUv) * 4;
	this->PostVBView.StrideInBytes = sizeof(VertexPosUv);

	//�萔�o�b�t�@�̐���
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataSP) + 
			0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ, 
		nullptr,
		IID_PPV_ARGS(&this->PostConstBuff));

	assert(SUCCEEDED(result));

	D3D12_RESOURCE_DESC texturedesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_R8G8B8A8_UNORM,
		win_width,
		(UINT)win_hight,
		1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET
	);

	//�e�N�X�`���o�b�t�@����
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK,
			D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&texturedesc,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		&CD3DX12_CLEAR_VALUE(DXGI_FORMAT_R8G8B8A8_UNORM, clearColor),
		IID_PPV_ARGS(&texbuff));

	assert(SUCCEEDED(result));

	const UINT pixelcount = win_width * win_hight;

	const UINT rowPitch = sizeof(UINT) * win_width;

	const UINT depthPitch = rowPitch * win_hight;

	UINT* img = new UINT[pixelcount];
	for (int i = 0; i < pixelcount; i++) { img[i] = 0xff0000ff; }

	result = texbuff->WriteToSubresource(0, nullptr,
		img, rowPitch, depthPitch);
	assert(SUCCEEDED(result));

	delete[] img;

	D3D12_DESCRIPTOR_HEAP_DESC srvDescHeapDesc = {};
	srvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvDescHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srvDescHeapDesc.NumDescriptors = 1;

	result = dev->CreateDescriptorHeap(&srvDescHeapDesc, IID_PPV_ARGS(&descHeapSRV));
	assert(SUCCEEDED(result));

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	dev->CreateShaderResourceView(texbuff.Get(),
		&srvDesc,
		descHeapSRV->GetCPUDescriptorHandleForHeapStart());

	D3D12_DESCRIPTOR_HEAP_DESC rtvDescHeapDesc{};
	rtvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvDescHeapDesc.NumDescriptors = 1;

	result = dev->CreateDescriptorHeap(&rtvDescHeapDesc, IID_PPV_ARGS(&descHeapRTV));
	assert(SUCCEEDED(result));

	dev->CreateRenderTargetView(texbuff.Get(),
		nullptr,
		descHeapRTV->GetCPUDescriptorHandleForHeapStart());

	CD3DX12_RESOURCE_DESC depthResDesc =
		CD3DX12_RESOURCE_DESC::Tex2D(
			DXGI_FORMAT_D32_FLOAT,
			win_width,
			win_hight,
			1, 0,
			1, 0,
			D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL
		);

	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&depthResDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0),
		IID_PPV_ARGS(&depthbuff));
	assert(SUCCEEDED(result));

	D3D12_DESCRIPTOR_HEAP_DESC DescHeapDesc{};
	DescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	DescHeapDesc.NumDescriptors = 1;

	result = dev->CreateDescriptorHeap(&DescHeapDesc, IID_PPV_ARGS(&descheapDSV));
	assert(SUCCEEDED(result));

	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	dev->CreateDepthStencilView(depthbuff.Get(),
		&dsvDesc,
		descheapDSV->GetCPUDescriptorHandleForHeapStart());

	CreateGraphicsPipelineState(dev);
}

void PostEffect::Draw(ID3D12GraphicsCommandList* cmdList, TexManager* texture, ID3D12Device* dev)
{
	//�萔�o�b�t�@�Ƀf�[�^�]��
	ConstBufferDataSP* constMap = nullptr;
	HRESULT result = this->PostConstBuff->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result)) {
		constMap->color = XMFLOAT4(1, 1, 1, 1);
		constMap->mat = XMMatrixIdentity();
		this->PostConstBuff->Unmap(0, nullptr);
	}

	cmdList->SetPipelineState(PostPipelinestate.Get());

	cmdList->SetGraphicsRootSignature(PostRootsignature.Get());

	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//ID3D12DescriptorHeap* ppHeaps[] = { texture->descHeap.Get() };
	ID3D12DescriptorHeap* ppHeaps[] = { descHeapSRV.Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	//���_�o�b�t�@���Z�b�g
	cmdList->IASetVertexBuffers(0, 1, &PostVBView);

	//�萔�o�b�t�@���Z�b�g
	cmdList->SetGraphicsRootConstantBufferView(0, PostConstBuff->GetGPUVirtualAddress());

	//�V�F�[�_�[���\�[�X�r���[���Z�b�g
	cmdList->SetGraphicsRootDescriptorTable(1, descHeapSRV->GetGPUDescriptorHandleForHeapStart());
	//cmdList->SetGraphicsRootDescriptorTable(1,texture->FindReturnSRV(texnumber, dev));

	//�`��R�}���h
	cmdList->DrawInstanced(4, 1, 0, 0);
}

void PostEffect::PreDrawScene(ID3D12GraphicsCommandList* cmdlist)
{
	cmdlist->ResourceBarrier(1,
		&CD3DX12_RESOURCE_BARRIER::Transition(texbuff.Get(),
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
			D3D12_RESOURCE_STATE_RENDER_TARGET));

	D3D12_CPU_DESCRIPTOR_HANDLE rtvH =
		descHeapRTV->GetCPUDescriptorHandleForHeapStart();

	D3D12_CPU_DESCRIPTOR_HANDLE dsvH =
		descheapDSV->GetCPUDescriptorHandleForHeapStart();

	cmdlist->OMSetRenderTargets(1, &rtvH, false, &dsvH);

	cmdlist->RSSetViewports(1, &CD3DX12_VIEWPORT(0.0f, 0.0f,
		win_width, win_hight));

	cmdlist->RSSetScissorRects(1, &CD3DX12_RECT(0, 0, win_width, win_hight));

	cmdlist->ClearRenderTargetView(rtvH, clearColor, 0, nullptr);

	cmdlist->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}

void PostEffect::PostDrawScene(ID3D12GraphicsCommandList* cmdlist)
{
	cmdlist->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(texbuff.Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));
}

void PostEffect::CreateGraphicsPipelineState(ID3D12Device* dev)
{
	HRESULT result;

	ComPtr<ID3DBlob> vsBlob = nullptr; // ���_�V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> psBlob = nullptr; // �s�N�Z���V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> errorBlob = nullptr; // �G���[�I�u�W�F�N�g

	// ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/shaders/PostEffectTestVS.hlsl",  // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "vs_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&vsBlob, &errorBlob);

	// �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/shaders/PostEffectTestPS.hlsl",   // �V�F�[�_�t�@�C����
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
		assert(0);
	}

	//���_���C�A�E�g
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
	assert(SUCCEEDED(result));

	result = dev->CreateRootSignature(0, rootSigBlob->GetBufferPointer(),
		rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&PostRootsignature));
	assert(SUCCEEDED(result));

	//�p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
	gpipeline.pRootSignature = PostRootsignature.Get();

	//�p�C�v���C���X�e�[�g�̐���
	result = dev->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&PostPipelinestate));
	assert(SUCCEEDED(result));
}
