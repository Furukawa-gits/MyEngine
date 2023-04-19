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

	//パイプライン生成
	setPipelineStateSprite();

	matprojection = XMMatrixOrthographicOffCenterLH(
		0.0f, (float)windowWidth, (float)windowHight, 0.0f, 0.0f, 1.0f);
}

//グラフィックスパイプライン生成
void SingleSprite::setPipelineStateSprite()
{
	if (spritePipelinestate != NULL && spriteRootsignature != NULL)
	{
		return;
	}

	HRESULT result;

	ComPtr<ID3DBlob> vsBlob = nullptr; // 頂点シェーダオブジェクト
	ComPtr<ID3DBlob> psBlob = nullptr; // ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob> errorBlob = nullptr; // エラーオブジェクト

	// 頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/shaders/SpriteVS.hlsl",  // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "vs_5_0", // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&vsBlob, &errorBlob);

	// ピクセルシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/shaders/SpritePS.hlsl",   // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "ps_5_0", // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&psBlob, &errorBlob);

	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	//頂点シェーダーに渡すための頂点データを整える
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

	// グラフィックスパイプラインステート設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};

	//頂点シェーダ、ピクセルシェーダをパイプラインに設定
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

	//サンプルマスクとラスタライザステートの設定
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; //標準設定
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

	//ブレンドステートの設定
	D3D12_RENDER_TARGET_BLEND_DESC& blenddsec = gpipeline.BlendState.RenderTarget[0];
	blenddsec.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	//ブレンド共通設定
	blenddsec.BlendEnable = true;
	blenddsec.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddsec.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddsec.DestBlendAlpha = D3D12_BLEND_ZERO;

	//合成設定
	blenddsec.BlendOp = D3D12_BLEND_OP_ADD;
	blenddsec.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddsec.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	//頂点レイアウトの設定
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	//図形の形状を三角形に設定
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	//その他の設定
	gpipeline.NumRenderTargets = 1; //描画対象は1つ
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; //0～255指定のRGBA
	gpipeline.SampleDesc.Count = 1; //1ピクセルにつき1回サンプリング

	//デプスステンシルステートの設定
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	gpipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;

	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	//デスクリプタレンジの設定
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;

	//デスクリプタテーブルの設定
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

	//ルートパラメータの設定
	CD3DX12_ROOT_PARAMETER rootparams[2];
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);//定数バッファビューとして初期化
	rootparams[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);

	//テクスチャサンプラーの設定
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc{};
	samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);

	//ルートシグネチャの生成
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob;
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc,
		D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(),
		rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&spriteRootsignature));

	//パイプラインにルートシグネチャをセット
	gpipeline.pRootSignature = spriteRootsignature.Get();

	//パイプラインステートの生成
	result = device->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&spritePipelinestate));
}

//スプライト単体頂点バッファの転送
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

	//切り抜き処理
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

	//頂点バッファへのデータ転送
	VertexPosUv* vertMap = nullptr;
	result = this->spriteVertBuff->Map(0, nullptr, (void**)&vertMap);
	memcpy(vertMap, vertices, sizeof(vertices));
	this->spriteVertBuff->Unmap(0, nullptr);
}

//テクスチャ読み込み
void SingleSprite::loadTexture(const std::string& filename)
{
	HRESULT result;

	const std::string baseDirectory = "Resources/Image/";

	std::string fullpath = baseDirectory + filename;

	wchar_t wfullpath[128];
	MultiByteToWideChar(CP_ACP, 0, fullpath.c_str(), -1, wfullpath, _countof(wfullpath));

	//WICテクスチャのロード
	TexMetadata metadata{};
	ScratchImage scratchImg{};

	result = LoadFromWICFile(
		wfullpath,
		WIC_FLAGS_NONE,
		&metadata, scratchImg);

	const Image* img = scratchImg.GetImage(0, 0, 0);

	//テクスチャバッファリソース設定
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

	//テクスチャバッファ生成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&this->texbuff));

	//テクスチャバッファへのデータ転送
	result = this->texbuff->WriteToSubresource(
		0,
		nullptr,
		img->pixels,
		(UINT)img->rowPitch,
		(UINT)img->slicePitch);

	//シェーダーリソースビュー設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	//シェーダーリソースビュー作成
	device->CreateShaderResourceView(this->texbuff.Get(),
		&srvDesc,
		CD3DX12_CPU_DESCRIPTOR_HANDLE(this->descHeapSRV->GetCPUDescriptorHandleForHeapStart(), 0,
			device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
		)
	);
}

//スプライト生成
void SingleSprite::generateSprite(
	const std::string& filename,
	bool sizeFlag,
	bool isFlipX,
	bool isFlipY,
	bool iscutout)
{
	loadTexture(filename);

	HRESULT result = S_FALSE;

	//頂点データ
	VertexPosUv vertices[] =
	{
		{{  0.0f,100.0f,0.0f},{0.0f,1.0f}},	//左下
		{{  0.0f,  0.0f,0.0f},{0.0f,0.0f}},	//左上
		{{100.0f,100.0f,0.0f},{1.0f,1.0f}},	//右下
		{{100.0f,  0.0f,0.0f},{1.0f,0.0f}}	//右上
	};

	//頂点バッファ生成
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

	//バッファへのデータ転送
	spriteTransferVertexBuffer(iscutout);

	//頂点バッファビューの作成
	this->spriteVBView.BufferLocation = this->spriteVertBuff->GetGPUVirtualAddress();
	this->spriteVBView.SizeInBytes = sizeof(vertices);
	this->spriteVBView.StrideInBytes = sizeof(vertices[0]);

	//定数バッファの生成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataSP) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&this->spriteConstBuff));

	//定数バッファにデータ転送
	ConstBufferDataSP* constMap = nullptr;
	result = this->spriteConstBuff->Map(0, nullptr, (void**)&constMap);
	constMap->color = XMFLOAT4(1, 1, 1, 1);
	constMap->mat = XMMatrixOrthographicOffCenterLH(
		0.0f, windowWidth, windowHight, 0.0f, 0.0f, 1.0f);
	this->spriteConstBuff->Unmap(0, nullptr);

	//入力用のサイズを設定
	setBaceSize(this->size);
}

//スプライト更新処理
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

//スプライト描画コマンド
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
	//頂点バッファをセット
	cmdList->IASetVertexBuffers(0, 1, &spriteVBView);

	//定数バッファをセット
	cmdList->SetGraphicsRootConstantBufferView(0, spriteConstBuff->GetGPUVirtualAddress());

	//シェーダーリソースビューをセット
	cmdList->SetGraphicsRootDescriptorTable(1, descHeapSRV->GetGPUDescriptorHandleForHeapStart());

	//描画コマンド
	cmdList->DrawInstanced(4, 1, 0, 0);
}

//マウス入力
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