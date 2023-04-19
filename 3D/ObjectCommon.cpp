#include"ObjectCommon.h"
#include<fstream>
#include<sstream>

#pragma region 3dオブジェクト共通
//バッファビュー取得_立方体
Viewes ObjectCommon::returnCubeViewes()
{
	Viewes views;

	//頂点情報配列
	Vertex verticis[] =
	{
		//前
		{{-10.0f,-10.0f,-10.0f},{},{0.0f,1.0f}},		//左下
		{{-10.0f, 10.0f,-10.0f},{},{0.0f,0.0f}},		//左上
		{{ 10.0f,-10.0f,-10.0f},{},{1.0f,1.0f}},		//右下
		{{ 10.0f, 10.0f,-10.0f},{},{1.0f,0.0f}},		//右上
		//後
		{{-10.0f, 10.0f, 10.0f},{},{0.0f,0.0f}},		//左上
		{{-10.0f,-10.0f, 10.0f},{},{0.0f,1.0f}},		//左下
		{{ 10.0f, 10.0f, 10.0f},{},{1.0f,0.0f}},		//右上
		{{ 10.0f,-10.0f, 10.0f},{},{1.0f,1.0f}},		//右下
		//左
		{{-10.0f,-10.0f,-10.0f},{},{0.0f,1.0f}},		//左下
		{{-10.0f,-10.0f, 10.0f},{},{0.0f,0.0f}},		//左上
		{{-10.0f, 10.0f,-10.0f},{},{1.0f,1.0f}},		//右下
		{{-10.0f, 10.0f, 10.0f},{},{1.0f,0.0f}},		//右上
		//右
		{{ 10.0f,-10.0f, 10.0f},{},{0.0f,1.0f}},		//左下
		{{ 10.0f,-10.0f,-10.0f},{},{0.0f,0.0f}},		//左上
		{{ 10.0f, 10.0f, 10.0f},{},{1.0f,1.0f}},		//右下
		{{ 10.0f, 10.0f,-10.0f},{},{1.0f,0.0f}},		//右上
		//下
		{{-10.0f,-10.0f, 10.0f},{},{0.0f,0.0f}},		//左上
		{{-10.0f,-10.0f,-10.0f},{},{0.0f,1.0f}},		//左下
		{{ 10.0f,-10.0f, 10.0f},{},{1.0f,0.0f}},		//右上
		{{ 10.0f,-10.0f,-10.0f},{},{1.0f,1.0f}},		//右下
		//上
		{{-10.0f, 10.0f,-10.0f},{},{0.0f,1.0f}},		//左下
		{{-10.0f, 10.0f, 10.0f},{},{0.0f,0.0f}},		//左上
		{{ 10.0f, 10.0f,-10.0f},{},{1.0f,1.0f}},		//右下
		{{ 10.0f, 10.0f, 10.0f},{},{1.0f,0.0f}}		//右上
	};

	//頂点インデックス配列
	unsigned short indices[] =
	{
		0,1,2,
		2,1,3,

		4,5,6,
		6,5,7,

		8,9,10,
		10,9,11,

		12,13,14,
		14,13,15,

		16,17,18,
		18,17,19,

		20,21,22,
		22,21,23
	};

	//法線ベクトルの計算
	for (int i = 0; i < _countof(indices) / 3; i++)
	{
		unsigned short indx0 = indices[i * 3 + 0];
		unsigned short indx1 = indices[i * 3 + 1];
		unsigned short indx2 = indices[i * 3 + 2];

		XMVECTOR p0 = XMLoadFloat3(&verticis[indx0].pos);
		XMVECTOR p1 = XMLoadFloat3(&verticis[indx1].pos);
		XMVECTOR p2 = XMLoadFloat3(&verticis[indx2].pos);

		XMVECTOR v1 = XMVectorSubtract(p1, p0);
		XMVECTOR v2 = XMVectorSubtract(p2, p0);

		XMVECTOR normal = XMVector3Cross(v1, v2);

		normal = XMVector3Normalize(normal);

		XMStoreFloat3(&verticis[indx0].normal, normal);
		XMStoreFloat3(&verticis[indx1].normal, normal);
		XMStoreFloat3(&verticis[indx2].normal, normal);
	}

	//頂点バッファ全体のサイズ
	UINT sizeVB = static_cast<UINT>(sizeof(Vertex) * _countof(verticis));

	//インデックスデータ全体のサイズ
	UINT sizeIB = static_cast<UINT>(sizeof(unsigned short)) * _countof(indices);


	directx->result = directx->dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(verticis)),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&views.vertBuff));

	//インデックスバッファの設定

	directx->result = directx->dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(indices)),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&views.indexBuff));

	// GPU上のバッファに対応した仮想メモリを取得
	Vertex* vertMap = nullptr;
	directx->result = views.vertBuff->Map(0, nullptr, (void**)&vertMap);

	// 全頂点に対して
	for (int i = 0; i < _countof(verticis); i++)
	{
		vertMap[i] = verticis[i];   // 座標をコピー
	}

	// マップを解除
	views.vertBuff->Unmap(0, nullptr);


	//インデックスバッファへのデータ転送
	//GPU上のバッファに対応した仮想メモリを取得
	unsigned short* indexMap = nullptr;
	directx->result = views.indexBuff->Map(0, nullptr, (void**)&indexMap);

	//全インデックスに対して
	for (int i = 0; i < _countof(indices); i++)
	{
		indexMap[i] = indices[i];
		views.Cubeindices[i] = indices[i];
	}

	//繋がりを解除
	views.indexBuff->Unmap(0, nullptr);

	// 頂点バッファビューの作成
	views.vbView.BufferLocation = views.vertBuff->GetGPUVirtualAddress();
	views.vbView.SizeInBytes = sizeVB;
	views.vbView.StrideInBytes = sizeof(Vertex);

	//インデックスバッファビューの作成
	views.idView.BufferLocation = views.indexBuff->GetGPUVirtualAddress();
	views.idView.Format = DXGI_FORMAT_R16_UINT;
	views.idView.SizeInBytes = sizeIB;

	return views;
}

//バッファビュー取得_三角錐
Viewes ObjectCommon::returnTripyramidViewes()
{
	Viewes views;

	//半径・円周率
	float R = 10.0f;
	float pi = 3.14159265f;

	//天井の高さ
	const float topHeight = -12.0f;

	//頂点情報配列
	Vertex verticis[] =
	{
		{{R * sinf(2 * pi / 3 * 0),R * cosf(2 * pi / 3 * 0),0},{},{1,1}},	//			0
		{{R * sinf(2 * pi / 3 * 0),R * cosf(2 * pi / 3 * 0),0},{},{1,1}},	//
		{{R * sinf(2 * pi / 3 * 0),R * cosf(2 * pi / 3 * 0),0},{},{1,1}},	//
		{{R * sinf(2 * pi / 3 * 0),R * cosf(2 * pi / 3 * 0),0},{},{1,1}},	//

		{{R * sinf(2 * pi / 3 * 1),R * cosf(2 * pi / 3 * 1),0},{},{0,1}},	//			1
		{{R * sinf(2 * pi / 3 * 1),R * cosf(2 * pi / 3 * 1),0},{},{0,1}},	//
		{{R * sinf(2 * pi / 3 * 1),R * cosf(2 * pi / 3 * 1),0},{},{0,1}},	//
		{{R * sinf(2 * pi / 3 * 1),R * cosf(2 * pi / 3 * 1),0},{},{0,1}},	//

		{{R * sinf(2 * pi / 3 * 2),R * cosf(2 * pi / 3 * 2),0},{},{1,0}},	//			2
		{{R * sinf(2 * pi / 3 * 2),R * cosf(2 * pi / 3 * 2),0},{},{1,0}},	//
		{{R * sinf(2 * pi / 3 * 2),R * cosf(2 * pi / 3 * 2),0},{},{1,0}},	//
		{{R * sinf(2 * pi / 3 * 2),R * cosf(2 * pi / 3 * 2),0},{},{1,0}},	//

		{{0,0,0},{},{0,1}},													//			3
		{{0,0,0},{},{0,1}},													//
		{{0,0,0},{},{0,1}},													//

		{{0,0,topHeight},{},{0,0}},											//			4
		{ {0,0,topHeight},{},{0,0}},										//
		{ {0,0,topHeight},{},{0,0}}											//

	};

	//頂点インデックス配列
	unsigned short indices[] =
	{
		/*1,0,3,
		2,1,3,
		0,2,3,

		0,1,4,
		1,2,4,
		2,0,4*/

		4,0,12,
		8,5,13,
		1,9,14,

		2,6,15,
		7,10,16,
		11,3,17

	};

	//法線ベクトルの計算
	for (int i = 0; i < _countof(indices) / 3; i++)
	{
		unsigned short indx0 = indices[i * 3 + 0];
		unsigned short indx1 = indices[i * 3 + 1];
		unsigned short indx2 = indices[i * 3 + 2];

		XMVECTOR p0 = XMLoadFloat3(&verticis[indx0].pos);
		XMVECTOR p1 = XMLoadFloat3(&verticis[indx1].pos);
		XMVECTOR p2 = XMLoadFloat3(&verticis[indx2].pos);

		XMVECTOR v1 = XMVectorSubtract(p1, p0);
		XMVECTOR v2 = XMVectorSubtract(p2, p0);

		XMVECTOR normal = XMVector3Cross(v1, v2);

		normal = XMVector3Normalize(normal);

		XMStoreFloat3(&verticis[indx0].normal, normal);
		XMStoreFloat3(&verticis[indx1].normal, normal);
		XMStoreFloat3(&verticis[indx2].normal, normal);
	}

	//頂点バッファ全体のサイズ
	UINT sizeVB = static_cast<UINT>(sizeof(Vertex) * _countof(verticis));

	//インデックスデータ全体のサイズ
	UINT sizeIB = static_cast<UINT>(sizeof(unsigned short)) * _countof(indices);

	//頂点バッファの生成
	directx->result = directx->dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(verticis)),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&views.vertBuff));

	//インデックスバッファの設定
	directx->result = directx->dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(indices)),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&views.indexBuff));

	// GPU上のバッファに対応した仮想メモリを取得
	Vertex* vertMap = nullptr;
	directx->result = views.vertBuff->Map(0, nullptr, (void**)&vertMap);

	// 全頂点に対して
	for (int i = 0; i < _countof(verticis); i++)
	{
		vertMap[i] = verticis[i];   // 座標をコピー
	}

	// マップを解除
	views.vertBuff->Unmap(0, nullptr);


	//インデックスバッファへのデータ転送
	//GPU上のバッファに対応した仮想メモリを取得
	unsigned short* indexMap = nullptr;
	directx->result = views.indexBuff->Map(0, nullptr, (void**)&indexMap);

	//全インデックスに対して
	for (int i = 0; i < _countof(indices); i++)
	{
		indexMap[i] = indices[i];
	}

	//繋がりを解除
	views.indexBuff->Unmap(0, nullptr);

	// 頂点バッファビューの作成
	views.vbView.BufferLocation = views.vertBuff->GetGPUVirtualAddress();
	views.vbView.SizeInBytes = sizeVB;
	views.vbView.StrideInBytes = sizeof(Vertex);

	//インデックスバッファビューの作成
	views.idView.BufferLocation = views.indexBuff->GetGPUVirtualAddress();
	views.idView.Format = DXGI_FORMAT_R16_UINT;
	views.idView.SizeInBytes = sizeIB;

	//全インデックスに対して
	for (int i = 0; i < _countof(indices); i++)
	{
		views.Triindices[i] = indices[i];
	}

	return views;
}

//バッファビュー取得_面
Viewes ObjectCommon::returnPlaneViewes()
{
	Viewes views;

	//頂点情報配列
	Vertex verticis[] =
	{
		{{-5.0f,-5.0f,0.0f},{0,0,1},{0,1}},
		{{-5.0f,+5.0f,0.0f},{0,0,1},{0,0}},
		{{+5.0f,-5.0f,0.0f},{0,0,1},{1,1}},
		{{+5.0f,+5.0f,0.0f},{0,0,1},{1,0}}
	};

	//頂点インデックス配列
	unsigned short indices[] =
	{
		0,1,2,
		2,1,3
	};

	//頂点バッファ全体のサイズ
	UINT sizeVB = static_cast<UINT>(sizeof(Vertex) * _countof(verticis));

	//インデックスデータ全体のサイズ
	UINT sizeIB = static_cast<UINT>(sizeof(unsigned short)) * _countof(indices);

	//頂点バッファの生成
	directx->result = directx->dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(verticis)),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&views.vertBuff));

	//インデックスバッファの設定
	directx->result = directx->dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(indices)),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&views.indexBuff));

	// GPU上のバッファに対応した仮想メモリを取得
	Vertex* vertMap = nullptr;
	directx->result = views.vertBuff->Map(0, nullptr, (void**)&vertMap);

	// 全頂点に対して
	for (int i = 0; i < _countof(verticis); i++)
	{
		vertMap[i] = verticis[i];   // 座標をコピー
	}

	// マップを解除
	views.vertBuff->Unmap(0, nullptr);


	//インデックスバッファへのデータ転送
	//GPU上のバッファに対応した仮想メモリを取得
	unsigned short* indexMap = nullptr;
	directx->result = views.indexBuff->Map(0, nullptr, (void**)&indexMap);

	//全インデックスに対して
	for (int i = 0; i < _countof(indices); i++)
	{
		indexMap[i] = indices[i];
	}

	//繋がりを解除
	views.indexBuff->Unmap(0, nullptr);

	// 頂点バッファビューの作成
	views.vbView.BufferLocation = views.vertBuff->GetGPUVirtualAddress();
	views.vbView.SizeInBytes = sizeVB;
	views.vbView.StrideInBytes = sizeof(Vertex);

	//インデックスバッファビューの作成
	views.idView.BufferLocation = views.indexBuff->GetGPUVirtualAddress();
	views.idView.Format = DXGI_FORMAT_R16_UINT;
	views.idView.SizeInBytes = sizeIB;

	//全インデックスに対して
	for (int i = 0; i < _countof(indices); i++)
	{
		views.Planeindices[i] = indices[i];
	}

	return views;
}

//マテリアルテクスチャ読み込み
void ObjectCommon::materialLoadTexture(Material* material, const std::string& filename)
{
	HRESULT result = S_FALSE;

	// WICテクスチャのロード
	TexMetadata metadata{};
	ScratchImage scratchImg{};

	wchar_t wfilepath[128];
	int iBufferSize = MultiByteToWideChar(CP_ACP, 0,
		filename.c_str(), -1, wfilepath, _countof(wfilepath));

	result = LoadFromWICFile(
		wfilepath, WIC_FLAGS_NONE,
		&metadata, scratchImg);


	const Image* img = scratchImg.GetImage(0, 0, 0);

	//テクスチャバッファリソース設定
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		metadata.format,
		metadata.width,
		(UINT)metadata.height,
		(UINT16)metadata.arraySize,
		(UINT16)metadata.mipLevels);

	//テクスチャバッファ生成
	result = directx->dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&this->texbuff[material->material_texnumber]));

	//テクスチャバッファへのデータ転送
	result = this->texbuff[material->material_texnumber]->WriteToSubresource(
		0,
		nullptr,
		img->pixels,
		(UINT)img->rowPitch,
		(UINT)img->slicePitch);

	//delete[] imageData;

	//シェーダーリソースビュー設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	//シェーダーリソースビュー作成
	directx->dev->CreateShaderResourceView(this->texbuff[material->material_texnumber].Get(),
		&srvDesc,
		CD3DX12_CPU_DESCRIPTOR_HANDLE(this->materialDescHeap->GetCPUDescriptorHandleForHeapStart(), material->material_texnumber,
			directx->dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
		)
	);
}

//マテリアル読み込み
void ObjectCommon::loadMaterial(Material* material, const std::string& filename)
{
	//ファイルストリーム
	std::ifstream file;

	const string directoryPath = "Resources/obj's/";

	//マテリアルファイルを開く
	file.open(filename);
	//ファイルオープン失敗をチェック
	if (file.fail())
	{
		return;
	}

	//1行ずつ読み込む
	string line;
	while (getline(file, line))
	{
		//１行分の文字列をストリームに変換
		std::istringstream line_stream(line);

		//半角スペース区切りで行の戦闘文字列を取得
		string key;
		getline(line_stream, key, ' ');

		//先頭のタブ文字は無視
		if (key[0] == '\t')
		{
			key.erase(key.begin());
		}

		if (key == "newmtl")
		{
			line_stream >> material->name;
		}

		if (key == "Ka")
		{
			line_stream >> material->ambient.x;
			line_stream >> material->ambient.y;
			line_stream >> material->ambient.z;
		}

		if (key == "Kd")
		{
			line_stream >> material->diffuse.x;
			line_stream >> material->diffuse.y;
			line_stream >> material->diffuse.z;
		}

		if (key == "Ks")
		{
			line_stream >> material->specular.x;
			line_stream >> material->specular.y;
			line_stream >> material->specular.z;
		}

		if (key == "map_Kd")
		{
			line_stream >> material->textureFilename;

			materialLoadTexture(material, directoryPath + material->textureFilename);
		}

	}
	file.close();
}

//バッファビュー取得_モデル
Viewes ObjectCommon::returnModelViewes(const char* Filename, int material_texnum)
{
	Viewes modelView;

	modelView.material.material_texnumber = material_texnum;

	//ファイルストリーム
	std::ifstream file;

	const string directoryPath = "Resources/obj's/";

	file.open(directoryPath + Filename);

	if (file.fail())
	{
		assert(0);
	}

	vector<XMFLOAT3> positions;
	vector<XMFLOAT3> normals;
	vector<XMFLOAT2> texcoords;

	//1行ずつ読み込み
	string line;
	while (getline(file, line))
	{
		std::istringstream line_stream(line);

		string key;
		getline(line_stream, key, ' ');

		if (key == "mtllib")
		{
			string filename;
			line_stream >> filename;
			loadMaterial(&modelView.material, directoryPath + filename);
		}

		if (key == "v")
		{
			XMFLOAT3 position{};
			line_stream >> position.x;
			line_stream >> position.y;
			line_stream >> position.z;
			//座標データに追加
			positions.emplace_back(position);
		}

		if (key == "vt")
		{
			XMFLOAT2 texcoord{};
			line_stream >> texcoord.x;
			line_stream >> texcoord.y;
			texcoord.y = 1.0f - texcoord.y;
			texcoords.emplace_back(texcoord);
		}

		if (key == "vn")
		{
			XMFLOAT3 normal{};
			line_stream >> normal.x;
			line_stream >> normal.y;
			line_stream >> normal.z;
			normals.emplace_back(normal);
		}

		if (key == "f")
		{
			int count = 0;
			string index_string;
			while (getline(line_stream, index_string, ' '))
			{
				count++;

				std::istringstream index_stream(index_string);
				unsigned short indexPosition, indexNormal, indexTexcoord;
				index_stream >> indexPosition;
				index_stream.seekg(1, ios_base::cur);
				index_stream >> indexTexcoord;
				index_stream.seekg(1, ios_base::cur);
				index_stream >> indexNormal;

				//頂点データの追加
				Vertex vertex{};
				vertex.pos = positions[indexPosition - 1];
				vertex.normal = normals[indexNormal - 1];
				vertex.uv = texcoords[indexTexcoord - 1];
				modelView.vertices.emplace_back(vertex);

				const uint16_t index0 = modelView.vertices.size() - 1;
				modelView.modelindices.emplace_back(index0);

				if (count > 3)
				{
					const uint16_t index1 = modelView.vertices.size() - 4;
					const uint16_t index2 = modelView.vertices.size() - 2;

					modelView.modelindices.emplace_back(index1);
					modelView.modelindices.emplace_back(index2);
				}
			}
		}
	}
	file.close();


#pragma region メッシュの読み込み
	UINT sizeVB = static_cast<UINT>(sizeof(Vertex) * modelView.vertices.size());

	UINT sizeIB = static_cast<UINT>(sizeof(unsigned short) * modelView.modelindices.size());

	// 頂点バッファ生成
	directx->result = directx->dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeVB),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&modelView.vertBuff));

	// インデックスバッファ生成
	directx->result = directx->dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeIB),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&modelView.indexBuff));

	// 頂点バッファへのデータ転送
	Vertex* vertMap = nullptr;
	directx->result = modelView.vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(directx->result)) {
		//memcpy(vertMap, vertices, sizeof(vertices));
		std::copy(modelView.vertices.begin(), modelView.vertices.end(), vertMap);
		modelView.vertBuff->Unmap(0, nullptr);
	}

	// インデックスバッファへのデータ転送
	unsigned short* indexMap = nullptr;
	directx->result = modelView.indexBuff->Map(0, nullptr, (void**)&indexMap);
	if (SUCCEEDED(directx->result)) {

		std::copy(modelView.modelindices.begin(), modelView.modelindices.end(), indexMap);

		modelView.indexBuff->Unmap(0, nullptr);
	}

	// 頂点バッファビューの作成
	modelView.vbView.BufferLocation = modelView.vertBuff->GetGPUVirtualAddress();
	modelView.vbView.SizeInBytes = sizeVB;
	modelView.vbView.StrideInBytes = sizeof(Vertex);

	// インデックスバッファビューの作成
	modelView.idView.BufferLocation = modelView.indexBuff->GetGPUVirtualAddress();
	modelView.idView.Format = DXGI_FORMAT_R16_UINT;
	modelView.idView.SizeInBytes = sizeIB;
#pragma endregion メッシュの読み込み

	return modelView;
}

//パイプライン設定
PipelineSet3D ObjectCommon::generatePipeLineState(ID3D12Device* dev)
{
	HRESULT result;

	ComPtr<ID3DBlob> vsBlob = nullptr; // 頂点シェーダオブジェクト
	ComPtr<ID3DBlob> psBlob = nullptr; // ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob> errorBlob = nullptr; // エラーオブジェクト

	// 頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/shaders/OBJVS.hlsl",  // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "vs_5_0", // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&vsBlob, &errorBlob);

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

	// ピクセルシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/shaders/OBJPS.hlsl",   // シェーダファイル名
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

	// 頂点レイアウト配列の宣言と設定
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{	//XYZ座標
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		}, // (1行で書いたほうが見やすい)

		{
			"NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},

		{//UV座標
			"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		}
	};

	// グラフィックスパイプラインステート設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};

	//頂点シェーダ・ピクセルシェーダ
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

	//標準設定(背面カリング、塗りつぶし、深度クリッピング有効)
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);

	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	gpipeline.NumRenderTargets = 1; // 描画対象は1つ
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0～255指定のRGBA
	gpipeline.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

	//デプスステンシルステートの設定
	//標準設定(深度テストを行う、書き込み許可、深度が小さければ合格)
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	//レンダーターゲットのブレンド設定
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

	//デスクリプタレンジの設定
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);


	//ルートパラメータの設定
	CD3DX12_ROOT_PARAMETER rootparams[3];
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[1].InitAsConstantBufferView(1, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[2].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);

	//テクスチャサンプラーの設定
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);

	//ルートシグネクチャ設定
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob;

	//バージョン自動判定でのシリアライズ
	result =
		D3DX12SerializeVersionedRootSignature(&rootSignatureDesc,
			D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);

	PipelineSet3D pipelineset;

	//ルートシグネクチャの生成
	result = dev->CreateRootSignature(0, rootSigBlob->GetBufferPointer(),
		rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&pipelineset.rootsignature));

	// パイプラインにルートシグネチャをセット
	gpipeline.pRootSignature = pipelineset.rootsignature.Get();

	//グラフィックスパイプラインステートの生成
	result = dev->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelineset.pipelinestate));

	return pipelineset;
}

//初期化
void ObjectCommon::init(directX* directx)
{
	this->directx = directx;

	this->pipelineSet = generatePipeLineState(this->directx->dev.Get());

	HRESULT result;

	//3Dオブジェクト設定構造体
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc{};

	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descHeapDesc.NumDescriptors = constantBufferNum + 1;

	result = this->directx->dev.Get()->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&basicDescHeap));

	// デスクリプタヒープを生成	
	D3D12_DESCRIPTOR_HEAP_DESC material_descHeapDesc = {};
	material_descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	material_descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//シェーダから見えるように
	material_descHeapDesc.NumDescriptors = 512; // シェーダーリソースビュー1つ
	result = this->directx->dev.Get()->CreateDescriptorHeap(&material_descHeapDesc, IID_PPV_ARGS(&materialDescHeap));//生成
	if (FAILED(result)) {
		assert(0);
		return;
	}

	// デスクリプタサイズを取得
	descriptorHandleIncrementSize = this->directx->dev.Get()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

//3Dオブジェクト共通描画処理
void ObjectCommon::object3DcommonBeginDraw()
{
	//3Dオブジェクトパイプライン・ルートシグネチャをセット
	this->directx->cmdList.Get()->SetPipelineState(this->pipelineSet.pipelinestate.Get());
	this->directx->cmdList.Get()->SetGraphicsRootSignature(this->pipelineSet.rootsignature.Get());

	//図形描画リスト・ストリップ
	this->directx->cmdList.Get()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
#pragma endregion 3dオブジェクト共通