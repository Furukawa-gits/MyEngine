#include"Primitive2D.h"

ID3D12Device* primitiveLine2D::device = nullptr;
ComPtr<ID3D12RootSignature> primitiveLine2D::lineRootsignature;
ComPtr<ID3D12PipelineState> primitiveLine2D::linePipelinestate;
XMMATRIX primitiveLine2D::matprojection;

void primitiveLine2D::SetStaticData(ID3D12Device* dev)
{
	device = dev;

	createGP();

	matprojection = XMMatrixOrthographicOffCenterLH(
		0.0f, (float)win_width, (float)win_hight, 0.0f, 0.0f, 1.0f);
}

void primitiveLine2D::lineTransferVertexBuffer()
{
	HRESULT result = S_FALSE;

	//頂点データ
	VertexPos vertices[] =
	{
		startPos,
		endPos
	};

	//頂点バッファへのデータ転送
	VertexPos* vertMap = nullptr;
	result = this->lineVertBuff->Map(0, nullptr, (void**)&vertMap);
	memcpy(vertMap, vertices, sizeof(vertices));
	this->lineVertBuff->Unmap(0, nullptr);
}

void primitiveLine2D::createGP()
{
	if (linePipelinestate != NULL && lineRootsignature != NULL)
	{
		return;
	}

	HRESULT result;

	ComPtr<ID3DBlob> vsBlob = nullptr; // 頂点シェーダオブジェクト
	ComPtr<ID3DBlob> psBlob = nullptr; // ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob> errorBlob = nullptr; // エラーオブジェクト

	// 頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/shaders/LineVS.hlsl",  // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "vs_5_0", // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&vsBlob, &errorBlob);

	// ピクセルシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/shaders/LinePS.hlsl",   // シェーダファイル名
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

	//図形の形状を線に設定
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;

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
		rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&lineRootsignature));

	//パイプラインにルートシグネチャをセット
	gpipeline.pRootSignature = lineRootsignature.Get();

	//パイプラインステートの生成
	result = device->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&linePipelinestate));
}

void primitiveLine2D::init()
{
	HRESULT result = S_FALSE;

	XMFLOAT3 a = { 0,0,0 };

	//頂点データ
	VertexPos vertices[] =
	{
		a,
		a
	};

	//頂点バッファ生成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(vertices)),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&lineVertBuff));

	//頂点バッファへのデータ転送
	VertexPos* vertMap = nullptr;
	result = this->lineVertBuff->Map(0, nullptr, (void**)&vertMap);
	memcpy(vertMap, vertices, sizeof(vertices));
	this->lineVertBuff->Unmap(0, nullptr);

	//頂点バッファビューの作成
	this->vbView.BufferLocation = this->lineVertBuff->GetGPUVirtualAddress();
	this->vbView.SizeInBytes = sizeof(vertices);
	this->vbView.StrideInBytes = sizeof(vertices);

	//定数バッファの生成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataL) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&this->lineConstBuff));

	//定数バッファにデータ転送
	ConstBufferDataL* constMap = nullptr;
	result = this->lineConstBuff->Map(0, nullptr, (void**)&constMap);
	constMap->color = XMFLOAT4(1, 1, 1, 1);
	constMap->startMat = XMMatrixOrthographicOffCenterLH(
		0.0f, win_width, win_hight, 0.0f, 0.0f, 1.0f);
	constMap->endMat = XMMatrixOrthographicOffCenterLH(
		0.0f, win_width, win_hight, 0.0f, 0.0f, 1.0f);
	this->lineConstBuff->Unmap(0, nullptr);
}

void primitiveLine2D::update()
{
	lineTransferVertexBuffer();

	startMatWorld = XMMatrixIdentity();

	startMatWorld *= XMMatrixTranslation(startPos.x, startPos.y, 0);

	endMatWorld = XMMatrixIdentity();

	endMatWorld *= XMMatrixTranslation(endPos.x, endPos.y, 0);

	HRESULT result = S_FALSE;

	//定数バッファにデータ転送
	ConstBufferDataL* constMap = nullptr;
	result = this->lineConstBuff->Map(0, nullptr, (void**)&constMap);
	constMap->color = color;
	constMap->startMat = endMatWorld * matprojection;
	constMap->endMat = startMatWorld * matprojection;
	this->lineConstBuff->Unmap(0, nullptr);
}

void primitiveLine2D::draw(ID3D12GraphicsCommandList* cmdList)
{
	cmdList->SetPipelineState(linePipelinestate.Get());

	cmdList->SetGraphicsRootSignature(lineRootsignature.Get());

	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);

	//頂点バッファをセット
	cmdList->IASetVertexBuffers(0, 1, &vbView);

	//定数バッファをセット
	cmdList->SetGraphicsRootConstantBufferView(0, lineConstBuff->GetGPUVirtualAddress());

	//描画コマンド
	cmdList->DrawInstanced(2, 1, 0, 0);
}
