#include"ObjectCommon.h"
#include<fstream>
#include<sstream>

#pragma region 3d�I�u�W�F�N�g����
//�o�b�t�@�r���[�擾_������
Viewes ObjectCommon::ReturnCubeViewes()
{
	Viewes views;

	//���_���z��
	Vertex verticis[] =
	{
		//�O
		{{-10.0f,-10.0f,-10.0f},{},{0.0f,1.0f}},		//����
		{{-10.0f, 10.0f,-10.0f},{},{0.0f,0.0f}},		//����
		{{ 10.0f,-10.0f,-10.0f},{},{1.0f,1.0f}},		//�E��
		{{ 10.0f, 10.0f,-10.0f},{},{1.0f,0.0f}},		//�E��
		//��
		{{-10.0f, 10.0f, 10.0f},{},{0.0f,0.0f}},		//����
		{{-10.0f,-10.0f, 10.0f},{},{0.0f,1.0f}},		//����
		{{ 10.0f, 10.0f, 10.0f},{},{1.0f,0.0f}},		//�E��
		{{ 10.0f,-10.0f, 10.0f},{},{1.0f,1.0f}},		//�E��
		//��
		{{-10.0f,-10.0f,-10.0f},{},{0.0f,1.0f}},		//����
		{{-10.0f,-10.0f, 10.0f},{},{0.0f,0.0f}},		//����
		{{-10.0f, 10.0f,-10.0f},{},{1.0f,1.0f}},		//�E��
		{{-10.0f, 10.0f, 10.0f},{},{1.0f,0.0f}},		//�E��
		//�E
		{{ 10.0f,-10.0f, 10.0f},{},{0.0f,1.0f}},		//����
		{{ 10.0f,-10.0f,-10.0f},{},{0.0f,0.0f}},		//����
		{{ 10.0f, 10.0f, 10.0f},{},{1.0f,1.0f}},		//�E��
		{{ 10.0f, 10.0f,-10.0f},{},{1.0f,0.0f}},		//�E��
		//��
		{{-10.0f,-10.0f, 10.0f},{},{0.0f,0.0f}},		//����
		{{-10.0f,-10.0f,-10.0f},{},{0.0f,1.0f}},		//����
		{{ 10.0f,-10.0f, 10.0f},{},{1.0f,0.0f}},		//�E��
		{{ 10.0f,-10.0f,-10.0f},{},{1.0f,1.0f}},		//�E��
		//��
		{{-10.0f, 10.0f,-10.0f},{},{0.0f,1.0f}},		//����
		{{-10.0f, 10.0f, 10.0f},{},{0.0f,0.0f}},		//����
		{{ 10.0f, 10.0f,-10.0f},{},{1.0f,1.0f}},		//�E��
		{{ 10.0f, 10.0f, 10.0f},{},{1.0f,0.0f}}		//�E��
	};

	//���_�C���f�b�N�X�z��
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

	//�@���x�N�g���̌v�Z
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

	//���_�o�b�t�@�S�̂̃T�C�Y
	UINT sizeVB = static_cast<UINT>(sizeof(Vertex) * _countof(verticis));

	//�C���f�b�N�X�f�[�^�S�̂̃T�C�Y
	UINT sizeIB = static_cast<UINT>(sizeof(unsigned short)) * _countof(indices);


	directx->result = directx->dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(verticis)),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&views.vertBuff));

	//�C���f�b�N�X�o�b�t�@�̐ݒ�

	directx->result = directx->dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(indices)),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&views.indexBuff));

	// GPU��̃o�b�t�@�ɑΉ��������z���������擾
	Vertex* vertMap = nullptr;
	directx->result = views.vertBuff->Map(0, nullptr, (void**)&vertMap);

	// �S���_�ɑ΂���
	for (int i = 0; i < _countof(verticis); i++)
	{
		vertMap[i] = verticis[i];   // ���W���R�s�[
	}

	// �}�b�v������
	views.vertBuff->Unmap(0, nullptr);


	//�C���f�b�N�X�o�b�t�@�ւ̃f�[�^�]��
	//GPU��̃o�b�t�@�ɑΉ��������z���������擾
	unsigned short* indexMap = nullptr;
	directx->result = views.indexBuff->Map(0, nullptr, (void**)&indexMap);

	//�S�C���f�b�N�X�ɑ΂���
	for (int i = 0; i < _countof(indices); i++)
	{
		indexMap[i] = indices[i];
		views.Cubeindices[i] = indices[i];
	}

	//�q���������
	views.indexBuff->Unmap(0, nullptr);

	// ���_�o�b�t�@�r���[�̍쐬
	views.vbView.BufferLocation = views.vertBuff->GetGPUVirtualAddress();
	views.vbView.SizeInBytes = sizeVB;
	views.vbView.StrideInBytes = sizeof(Vertex);

	//�C���f�b�N�X�o�b�t�@�r���[�̍쐬
	views.idView.BufferLocation = views.indexBuff->GetGPUVirtualAddress();
	views.idView.Format = DXGI_FORMAT_R16_UINT;
	views.idView.SizeInBytes = sizeIB;

	return views;
}

//�o�b�t�@�r���[�擾_�O�p��
Viewes ObjectCommon::ReturnTripyramidViewes()
{
	Viewes views;

	//���a�E�~����
	float R = 10.0f;
	float pi = 3.14159265f;

	//�V��̍���
	const float topHeight = -12.0f;

	//���_���z��
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

	//���_�C���f�b�N�X�z��
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

	//�@���x�N�g���̌v�Z
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

	//���_�o�b�t�@�S�̂̃T�C�Y
	UINT sizeVB = static_cast<UINT>(sizeof(Vertex) * _countof(verticis));

	//�C���f�b�N�X�f�[�^�S�̂̃T�C�Y
	UINT sizeIB = static_cast<UINT>(sizeof(unsigned short)) * _countof(indices);

	//���_�o�b�t�@�̐���
	directx->result = directx->dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(verticis)),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&views.vertBuff));

	//�C���f�b�N�X�o�b�t�@�̐ݒ�
	directx->result = directx->dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(indices)),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&views.indexBuff));

	// GPU��̃o�b�t�@�ɑΉ��������z���������擾
	Vertex* vertMap = nullptr;
	directx->result = views.vertBuff->Map(0, nullptr, (void**)&vertMap);

	// �S���_�ɑ΂���
	for (int i = 0; i < _countof(verticis); i++)
	{
		vertMap[i] = verticis[i];   // ���W���R�s�[
	}

	// �}�b�v������
	views.vertBuff->Unmap(0, nullptr);


	//�C���f�b�N�X�o�b�t�@�ւ̃f�[�^�]��
	//GPU��̃o�b�t�@�ɑΉ��������z���������擾
	unsigned short* indexMap = nullptr;
	directx->result = views.indexBuff->Map(0, nullptr, (void**)&indexMap);

	//�S�C���f�b�N�X�ɑ΂���
	for (int i = 0; i < _countof(indices); i++)
	{
		indexMap[i] = indices[i];
	}

	//�q���������
	views.indexBuff->Unmap(0, nullptr);

	// ���_�o�b�t�@�r���[�̍쐬
	views.vbView.BufferLocation = views.vertBuff->GetGPUVirtualAddress();
	views.vbView.SizeInBytes = sizeVB;
	views.vbView.StrideInBytes = sizeof(Vertex);

	//�C���f�b�N�X�o�b�t�@�r���[�̍쐬
	views.idView.BufferLocation = views.indexBuff->GetGPUVirtualAddress();
	views.idView.Format = DXGI_FORMAT_R16_UINT;
	views.idView.SizeInBytes = sizeIB;

	//�S�C���f�b�N�X�ɑ΂���
	for (int i = 0; i < _countof(indices); i++)
	{
		views.Triindices[i] = indices[i];
	}

	return views;
}

//�o�b�t�@�r���[�擾_��
Viewes ObjectCommon::ReturnPlaneViewes()
{
	Viewes views;

	//���_���z��
	Vertex verticis[] =
	{
		{{-5.0f,-5.0f,0.0f},{0,0,1},{0,1}},
		{{-5.0f,+5.0f,0.0f},{0,0,1},{0,0}},
		{{+5.0f,-5.0f,0.0f},{0,0,1},{1,1}},
		{{+5.0f,+5.0f,0.0f},{0,0,1},{1,0}}
	};

	//���_�C���f�b�N�X�z��
	unsigned short indices[] =
	{
		0,1,2,
		2,1,3
	};

	//���_�o�b�t�@�S�̂̃T�C�Y
	UINT sizeVB = static_cast<UINT>(sizeof(Vertex) * _countof(verticis));

	//�C���f�b�N�X�f�[�^�S�̂̃T�C�Y
	UINT sizeIB = static_cast<UINT>(sizeof(unsigned short)) * _countof(indices);

	//���_�o�b�t�@�̐���
	directx->result = directx->dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(verticis)),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&views.vertBuff));

	//�C���f�b�N�X�o�b�t�@�̐ݒ�
	directx->result = directx->dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(indices)),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&views.indexBuff));

	// GPU��̃o�b�t�@�ɑΉ��������z���������擾
	Vertex* vertMap = nullptr;
	directx->result = views.vertBuff->Map(0, nullptr, (void**)&vertMap);

	// �S���_�ɑ΂���
	for (int i = 0; i < _countof(verticis); i++)
	{
		vertMap[i] = verticis[i];   // ���W���R�s�[
	}

	// �}�b�v������
	views.vertBuff->Unmap(0, nullptr);


	//�C���f�b�N�X�o�b�t�@�ւ̃f�[�^�]��
	//GPU��̃o�b�t�@�ɑΉ��������z���������擾
	unsigned short* indexMap = nullptr;
	directx->result = views.indexBuff->Map(0, nullptr, (void**)&indexMap);

	//�S�C���f�b�N�X�ɑ΂���
	for (int i = 0; i < _countof(indices); i++)
	{
		indexMap[i] = indices[i];
	}

	//�q���������
	views.indexBuff->Unmap(0, nullptr);

	// ���_�o�b�t�@�r���[�̍쐬
	views.vbView.BufferLocation = views.vertBuff->GetGPUVirtualAddress();
	views.vbView.SizeInBytes = sizeVB;
	views.vbView.StrideInBytes = sizeof(Vertex);

	//�C���f�b�N�X�o�b�t�@�r���[�̍쐬
	views.idView.BufferLocation = views.indexBuff->GetGPUVirtualAddress();
	views.idView.Format = DXGI_FORMAT_R16_UINT;
	views.idView.SizeInBytes = sizeIB;

	//�S�C���f�b�N�X�ɑ΂���
	for (int i = 0; i < _countof(indices); i++)
	{
		views.Planeindices[i] = indices[i];
	}

	return views;
}

//�}�e���A���e�N�X�`���ǂݍ���
void ObjectCommon::Material_Loadtexture(Material* material, const std::string& filename)
{
	HRESULT result = S_FALSE;

	// WIC�e�N�X�`���̃��[�h
	TexMetadata metadata{};
	ScratchImage scratchImg{};

	wchar_t wfilepath[128];
	int iBufferSize = MultiByteToWideChar(CP_ACP, 0,
		filename.c_str(), -1, wfilepath, _countof(wfilepath));

	result = LoadFromWICFile(
		wfilepath, WIC_FLAGS_NONE,
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
	result = directx->dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&this->texbuff[material->material_texnumber]));

	//�e�N�X�`���o�b�t�@�ւ̃f�[�^�]��
	result = this->texbuff[material->material_texnumber]->WriteToSubresource(
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
	directx->dev->CreateShaderResourceView(this->texbuff[material->material_texnumber].Get(),
		&srvDesc,
		CD3DX12_CPU_DESCRIPTOR_HANDLE(this->Material_descHeap->GetCPUDescriptorHandleForHeapStart(), material->material_texnumber,
			directx->dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
		)
	);
}

//�}�e���A���ǂݍ���
void ObjectCommon::LoadMaterial(Material* material, const std::string& filename)
{
	//�t�@�C���X�g���[��
	std::ifstream file;

	const string directoryPath = "Resources/obj's/";

	//�}�e���A���t�@�C�����J��
	file.open(filename);
	//�t�@�C���I�[�v�����s���`�F�b�N
	if (file.fail())
	{
		return;
	}

	//1�s���ǂݍ���
	string line;
	while (getline(file, line))
	{
		//�P�s���̕�������X�g���[���ɕϊ�
		std::istringstream line_stream(line);

		//���p�X�y�[�X��؂�ōs�̐퓬��������擾
		string key;
		getline(line_stream, key, ' ');

		//�擪�̃^�u�����͖���
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

			Material_Loadtexture(material, directoryPath + material->textureFilename);
		}

	}
	file.close();
}

//�o�b�t�@�r���[�擾_���f��
Viewes ObjectCommon::ReturnModelViewes(const char* Filename, int material_texnum)
{
	Viewes modelView;

	modelView.material.material_texnumber = material_texnum;

	//�t�@�C���X�g���[��
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

	//1�s���ǂݍ���
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
			LoadMaterial(&modelView.material, directoryPath + filename);
		}

		if (key == "v")
		{
			XMFLOAT3 position{};
			line_stream >> position.x;
			line_stream >> position.y;
			line_stream >> position.z;
			//���W�f�[�^�ɒǉ�
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

				//���_�f�[�^�̒ǉ�
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


#pragma region ���b�V���̓ǂݍ���
	UINT sizeVB = static_cast<UINT>(sizeof(Vertex) * modelView.vertices.size());

	UINT sizeIB = static_cast<UINT>(sizeof(unsigned short) * modelView.modelindices.size());

	// ���_�o�b�t�@����
	directx->result = directx->dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeVB),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&modelView.vertBuff));

	// �C���f�b�N�X�o�b�t�@����
	directx->result = directx->dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeIB),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&modelView.indexBuff));

	// ���_�o�b�t�@�ւ̃f�[�^�]��
	Vertex* vertMap = nullptr;
	directx->result = modelView.vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(directx->result)) {
		//memcpy(vertMap, vertices, sizeof(vertices));
		std::copy(modelView.vertices.begin(), modelView.vertices.end(), vertMap);
		modelView.vertBuff->Unmap(0, nullptr);
	}

	// �C���f�b�N�X�o�b�t�@�ւ̃f�[�^�]��
	unsigned short* indexMap = nullptr;
	directx->result = modelView.indexBuff->Map(0, nullptr, (void**)&indexMap);
	if (SUCCEEDED(directx->result)) {

		std::copy(modelView.modelindices.begin(), modelView.modelindices.end(), indexMap);

		modelView.indexBuff->Unmap(0, nullptr);
	}

	// ���_�o�b�t�@�r���[�̍쐬
	modelView.vbView.BufferLocation = modelView.vertBuff->GetGPUVirtualAddress();
	modelView.vbView.SizeInBytes = sizeVB;
	modelView.vbView.StrideInBytes = sizeof(Vertex);

	// �C���f�b�N�X�o�b�t�@�r���[�̍쐬
	modelView.idView.BufferLocation = modelView.indexBuff->GetGPUVirtualAddress();
	modelView.idView.Format = DXGI_FORMAT_R16_UINT;
	modelView.idView.SizeInBytes = sizeIB;
#pragma endregion ���b�V���̓ǂݍ���

	return modelView;
}

//�p�C�v���C���ݒ�
PipelineSet3D ObjectCommon::GeneratePipeLineState(ID3D12Device* dev)
{
	HRESULT result;

	ComPtr<ID3DBlob> vsBlob = nullptr; // ���_�V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> psBlob = nullptr; // �s�N�Z���V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> errorBlob = nullptr; // �G���[�I�u�W�F�N�g

	// ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/shaders/OBJVS.hlsl",  // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "vs_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&vsBlob, &errorBlob);

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

	// �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/shaders/OBJPS.hlsl",   // �V�F�[�_�t�@�C����
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

	// ���_���C�A�E�g�z��̐錾�Ɛݒ�
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{	//XYZ���W
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		}, // (1�s�ŏ������ق������₷��)

		{
			"NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},

		{//UV���W
			"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		}
	};

	// �O���t�B�b�N�X�p�C�v���C���X�e�[�g�ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};

	//���_�V�F�[�_�E�s�N�Z���V�F�[�_
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

	//�W���ݒ�(�w�ʃJ�����O�A�h��Ԃ��A�[�x�N���b�s���O�L��)
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);

	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	gpipeline.NumRenderTargets = 1; // �`��Ώۂ�1��
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0�`255�w���RGBA
	gpipeline.SampleDesc.Count = 1; // 1�s�N�Z���ɂ�1��T���v�����O

	//�f�v�X�X�e���V���X�e�[�g�̐ݒ�
	//�W���ݒ�(�[�x�e�X�g���s���A�������݋��A�[�x����������΍��i)
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	//�����_�[�^�[�Q�b�g�̃u�����h�ݒ�
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

	//�f�X�N���v�^�����W�̐ݒ�
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);


	//���[�g�p�����[�^�̐ݒ�
	CD3DX12_ROOT_PARAMETER rootparams[3];
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[1].InitAsConstantBufferView(1, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[2].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);

	//�e�N�X�`���T���v���[�̐ݒ�
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);

	//���[�g�V�O�l�N�`���ݒ�
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob;

	//�o�[�W������������ł̃V���A���C�Y
	result =
		D3DX12SerializeVersionedRootSignature(&rootSignatureDesc,
			D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);

	PipelineSet3D pipelineset;

	//���[�g�V�O�l�N�`���̐���
	result = dev->CreateRootSignature(0, rootSigBlob->GetBufferPointer(),
		rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&pipelineset.rootsignature));

	// �p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
	gpipeline.pRootSignature = pipelineset.rootsignature.Get();

	//�O���t�B�b�N�X�p�C�v���C���X�e�[�g�̐���
	result = dev->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelineset.pipelinestate));

	return pipelineset;
}

//������
void ObjectCommon::init(directX* directx)
{
	this->directx = directx;

	this->pipelineset = GeneratePipeLineState(this->directx->dev.Get());

	HRESULT result;

	//3D�I�u�W�F�N�g�ݒ�\����
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc{};

	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descHeapDesc.NumDescriptors = constantBufferNum + 1;

	result = this->directx->dev.Get()->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&basicDescHeap));

	// �f�X�N���v�^�q�[�v�𐶐�	
	D3D12_DESCRIPTOR_HEAP_DESC material_descHeapDesc = {};
	material_descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	material_descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//�V�F�[�_���猩����悤��
	material_descHeapDesc.NumDescriptors = 512; // �V�F�[�_�[���\�[�X�r���[1��
	result = this->directx->dev.Get()->CreateDescriptorHeap(&material_descHeapDesc, IID_PPV_ARGS(&Material_descHeap));//����
	if (FAILED(result)) {
		assert(0);
		return;
	}

	// �f�X�N���v�^�T�C�Y���擾
	descriptorHandleIncrementSize = this->directx->dev.Get()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

//3D�I�u�W�F�N�g���ʕ`�揈��
void ObjectCommon::object3DcommonBeginDraw()
{
	//3D�I�u�W�F�N�g�p�C�v���C���E���[�g�V�O�l�`�����Z�b�g
	this->directx->cmdList.Get()->SetPipelineState(this->pipelineset.pipelinestate.Get());
	this->directx->cmdList.Get()->SetGraphicsRootSignature(this->pipelineset.rootsignature.Get());

	//�}�`�`�惊�X�g�E�X�g���b�v
	this->directx->cmdList.Get()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
#pragma endregion 3d�I�u�W�F�N�g����