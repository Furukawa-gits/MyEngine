#include "FbxLoader.h"
#include<cassert>

using namespace DirectX;

const std::string FbxLoader::baseDirectory = "Resources/";

const std::string FbxLoader::defaultTextureFileName = "white1x1.png";

FbxLoader* FbxLoader::GetInstance()
{
	static FbxLoader instance;
	return &instance;
}

void FbxLoader::Initilize(ID3D12Device* device)
{
	assert(fbxManager == nullptr);

	this->device = device;

	fbxManager = FbxManager::Create();

	FbxIOSettings* ios = FbxIOSettings::Create(fbxManager, IOSROOT);
	fbxManager->SetIOSettings(ios);

	fbxImporter = FbxImporter::Create(fbxManager, "");
}

Model* FbxLoader::LoadmodelFromFile(const string& modelName)
{
	const string directoryPath = baseDirectory + modelName + "/";

	const string filename = modelName + ".fbx";

	const string fullpath = directoryPath + filename;

	if (!fbxImporter->Initialize(fullpath.c_str(),
		-1, fbxManager->GetIOSettings()))
	{
		assert(0);
	}

	FbxScene* fbxscene = FbxScene::Create(fbxManager, "fbxscene");

	fbxImporter->Import(fbxscene);

	Model* model = new Model();
	model->name = modelName;

	int nodeCount = fbxscene->GetNodeCount();

	model->nodes.reserve(nodeCount);

	ParseNodeRecursive(model, fbxscene->GetRootNode());

	//fbxscene->Destroy();
	model->fbxScene = fbxscene;

	model->CreateBuffers(device);

	return model;
}

void FbxLoader::ParseNodeRecursive(Model* model, FbxNode* fbxnode, Node* parent)
{
	model->nodes.emplace_back();
	Node& node = model->nodes.back();

	node.name = fbxnode->GetName();

	FbxDouble3 rotation = fbxnode->LclRotation.Get();
	FbxDouble3 scaling = fbxnode->LclScaling.Get();
	FbxDouble3 translation = fbxnode->LclTranslation.Get();

	node.rotation = { (float)rotation[0],(float)rotation[1],(float)rotation[2],0.0f };
	node.scaling = { (float)scaling[0],(float)scaling[1],(float)scaling[2],0.0f };
	node.translation = { (float)translation[0],(float)translation[1],(float)translation[2],1.0f };

	node.rotation.m128_f32[0] = XMConvertToRadians(node.rotation.m128_f32[0]);
	node.rotation.m128_f32[1] = XMConvertToRadians(node.rotation.m128_f32[1]);
	node.rotation.m128_f32[2] = XMConvertToRadians(node.rotation.m128_f32[2]);

	XMMATRIX matScaling, matRotation, matTranslation;
	matScaling = XMMatrixScalingFromVector(node.scaling);
	matRotation = XMMatrixRotationRollPitchYawFromVector(node.rotation);
	matTranslation = XMMatrixTranslationFromVector(node.translation);

	node.transform = XMMatrixIdentity();
	node.transform *= matScaling;
	node.transform *= matRotation;
	node.transform *= matTranslation;

	node.globalTransform = node.transform;
	if (parent)
	{
		node.parent = parent;
		node.globalTransform *= parent->globalTransform;
	}

	FbxNodeAttribute* fbxnodeattribute = fbxnode->GetNodeAttribute();

	if (fbxnodeattribute)
	{
		if (fbxnodeattribute->GetAttributeType() == FbxNodeAttribute::eMesh)
		{
			model->meshNode = &node;
			ParseMesh(model, fbxnode);
		}
	}

	//再帰呼び出し
	for (int i = 0; i < fbxnode->GetChildCount(); i++)
	{
		ParseNodeRecursive(model, fbxnode->GetChild(i), &node);
	}
}
//メッシュ
void FbxLoader::ParseMesh(Model* model, FbxNode* fbxNode)
{
	//ノードのメッシュ読み取り
	FbxMesh* fbxMesh = fbxNode->GetMesh();
	//面を構成するデータの読み取り
	ParseMeshFaces(model, fbxMesh);
	//マテリアルの読み取り
	ParseMeshMaterial(model, fbxNode);
	//スキニング情報の読み取り
	ParseSkin(model, fbxMesh);
}
//頂点情報
void FbxLoader::ParseMeshVertices(Model* model, FbxMesh* fbxmesh)
{
	auto& vertices = model->vertices;

	const int controlpointsCount = fbxmesh->GetControlPointsCount();

	Model::VertexPosNormalUvSkin vert{};
	model->vertices.resize(controlpointsCount, vert);

	FbxVector4* pCoord = fbxmesh->GetControlPoints();

	for (int i = 0; i < controlpointsCount; i++)
	{
		Model::VertexPosNormalUvSkin& vertex = vertices[i];

		vertex.pos.x = (float)pCoord[i][0];
		vertex.pos.y = (float)pCoord[i][1];
		vertex.pos.z = (float)pCoord[i][2];
	}
}
//面情報
void FbxLoader::ParseMeshFaces(Model* model, FbxMesh* fbxMesh)
{
	auto& vertices = model->vertices;
	auto& indices = model->indices;

	assert(indices.size() == 0);

	const int polygonCount = fbxMesh->GetPolygonCount();

	const int textureUVCount = fbxMesh->GetTextureUVCount();

	const int controlpointsCount = fbxMesh->GetControlPointsCount();

	FbxStringList uvNames;
	fbxMesh->GetUVSetNames(uvNames);

	//頂点座標配列の先頭を取得
	FbxVector4* pCoord = fbxMesh->GetControlPoints();

	//vertices のサイズを ポリゴン数 * 1ポリゴン当たりの頂点数に変更
	Model::VertexPosNormalUvSkin vert{};

	for (int i = 0; i < polygonCount; i++)
	{
		const int polygonSize = fbxMesh->GetPolygonSize(i);
		if (polygonSize > 3)
		{
			assert(polygonSize <= 3);
		}

		for (int j = 0; j < polygonSize; j++)
		{
			int index = fbxMesh->GetPolygonVertex(i, j);
			assert(index >= 0);

			Model::VertexPosNormalUvSkin vertex = {};

			//頂点座標
			//頂点配列の先頭 + index
			vertex.pos.x = (float)(*(pCoord + index))[0];
			vertex.pos.y = (float)(*(pCoord + index))[1];
			vertex.pos.z = (float)(*(pCoord + index))[2];

			//法線
			FbxVector4 normal;
			if (fbxMesh->GetPolygonVertexNormal(i, j, normal))
			{
				vertex.normal.x = (float)normal[0];
				vertex.normal.y = (float)normal[1];
				vertex.normal.z = (float)normal[2];
			}

			//UV
			if (textureUVCount > 0)
			{
				FbxVector2 uvs;
				bool lUnmappedUV;

				if (fbxMesh->GetPolygonVertexUV(i, j, uvNames[0], uvs, lUnmappedUV))
				{
					vertex.uv.x = (float)uvs[0];
					vertex.uv.y = 1.0f - (float)uvs[1];
				}
			}

			//頂点とインデックスを動的に増やす
			indices.push_back(vertices.size());
			vertices.push_back(vertex);
		}
	}
}
//マテリアル
void FbxLoader::ParseMeshMaterial(Model* model, FbxNode* fbxNode)
{
	const int materialCount = fbxNode->GetMaterialCount();
	if (materialCount > 0)
	{
		FbxSurfaceMaterial* material = fbxNode->GetMaterial(0);

		bool textureLoaded = false;

		if (material)
		{
			if (material->GetClassId().Is(FbxSurfaceLambert::ClassId))
			{
				FbxSurfaceLambert* lambert = static_cast<FbxSurfaceLambert*>(material);

				FbxPropertyT<FbxDouble3> ambient = lambert->Ambient;
				model->ambient.x = (float)ambient.Get()[0];
				model->ambient.y = (float)ambient.Get()[1];
				model->ambient.z = (float)ambient.Get()[2];

				FbxPropertyT<FbxDouble3> diffuse = lambert->Diffuse;
				model->diffuse.x = (float)diffuse.Get()[0];
				model->diffuse.y = (float)diffuse.Get()[1];
				model->diffuse.z = (float)diffuse.Get()[2];

				const FbxProperty diffuseProperty =
					material->FindProperty(FbxSurfaceMaterial::sDiffuse);

				if (diffuseProperty.IsValid())
				{
					const FbxFileTexture* texture = diffuseProperty.GetSrcObject<FbxFileTexture>();
					if (texture)
					{
						const char* filepath = texture->GetFileName();

						string path_str(filepath);
						string name = ExtractFileName(path_str);

						LoadTexture(model, baseDirectory + model->name + "/" + name);
						textureLoaded = true;
					}
				}
			}
		}

		if (!textureLoaded)
		{
			LoadTexture(model, baseDirectory + defaultTextureFileName);
		}
	}
}
//テクスチャ読み込み
void FbxLoader::LoadTexture(Model* model, const std::string& fullpath)
{
	HRESULT result = S_FALSE;

	TexMetadata& metadata = model->metadata;
	ScratchImage& scratchImg = model->scratchimg;

	wchar_t wfilepath[128];
	MultiByteToWideChar(CP_ACP, 0, fullpath.c_str(), -1, wfilepath, _countof(wfilepath));
	result = LoadFromWICFile(
		wfilepath, WIC_FLAGS_NONE,
		&metadata, scratchImg
	);

	if (FAILED(result))
	{
		assert(0);
	}
}
//ディレクトリ込みのパスからファイル名抽出
std::string FbxLoader::ExtractFileName(const std::string& path)
{
	size_t pos1;

	pos1 = path.rfind('\\');
	if (pos1 != string::npos)
	{
		return path.substr(pos1 + 1, path.size() - pos1 - 1);
	}

	pos1 = path.rfind('/');
	if (pos1 != string::npos)
	{
		return path.substr(pos1 + 1, path.size() - pos1 - 1);
	}

	return path;
}

void FbxLoader::ConvertMatrixFromFbx(DirectX::XMMATRIX* dst, FbxAMatrix& src)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			dst->r[i].m128_f32[j] = (float)src.Get(i, j);
		}
	}
}

void FbxLoader::ParseSkin(Model* model, FbxMesh* fbxMesh)
{
	FbxSkin* fbxSkin =
		static_cast<FbxSkin*>(fbxMesh->GetDeformer(0,
			FbxDeformer::eSkin));

	if (fbxSkin == nullptr)
	{
		for (int i = 0; i < model->vertices.size(); i++)
		{
			model->vertices[i].boneIndex[0] = 0;
			model->vertices[i].boneWeight[0] = 1.0f;
		}

		return;
	}

	//ボーン配列の参照
	std::vector<Model::Bone>& bones = model->bones;

	int clusterCount = fbxSkin->GetClusterCount();
	bones.reserve(clusterCount);

	for (int i = 0; i < clusterCount; i++)
	{
		FbxCluster* fbxCluster = fbxSkin->GetCluster(i);

		const char* boneName = fbxCluster->GetLink()->GetName();

		bones.emplace_back(Model::Bone(boneName));
		Model::Bone& bone = bones.back();

		bone.fbxCluster = fbxCluster;

		FbxAMatrix fbxMat;
		fbxCluster->GetTransformLinkMatrix(fbxMat);

		XMMATRIX initialPose;
		ConvertMatrixFromFbx(&initialPose, fbxMat);

		bone.invInitialPose = XMMatrixInverse(nullptr, initialPose);
	}

	struct WeightSet {
		UINT index;
		float weight;
	};

	std::vector<std::list<WeightSet>> weightLists(model->vertices.size());

	for (int i = 0; i < clusterCount; i++)
	{
		FbxCluster* fbxCluster = fbxSkin->GetCluster(i);

		int controlPointIndicesCount = fbxCluster->GetControlPointIndicesCount();

		int* controlpointindices = fbxCluster->GetControlPointIndices();
		double* controlPointWeights = fbxCluster->GetControlPointWeights();

		for (int j = 0; j < controlPointIndicesCount; j++)
		{
			int vertIndex = controlpointindices[j];

			float weight = (float)controlPointWeights[j];

			weightLists[vertIndex].emplace_back(WeightSet{ (UINT)i,weight });
		}
	}

	auto& vertices = model->vertices;

	for (int i = 0; i < vertices.size(); i++)
	{
		auto& weightList = weightLists[i];

		weightList.sort(
			[](auto const& lhs, auto const rhs) {
				return lhs.weight > rhs.weight;
			});

		int weightArrayIndex = 0;

		for (auto& WeightSet : weightList)
		{
			vertices[i].boneIndex[weightArrayIndex] = WeightSet.index;
			vertices[i].boneWeight[weightArrayIndex] = WeightSet.weight;

			if (++weightArrayIndex >= Model::maxBoneIndices)
			{
				float weight = 0.0f;

				for (int j = 1; j < Model::maxBoneIndices; j++)
				{
					weight += vertices[i].boneWeight[j];
				}

				vertices[i].boneWeight[0] = 1.0f - weight;
				break;
			}
		}
	}
}

void FbxLoader::Finalize()
{
	fbxImporter->Destroy();
	fbxManager->Destroy();
}
