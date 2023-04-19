#pragma once
#include<string>
#include<DirectXMath.h>
#include <vector>
#include <DirectXTex.h>
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include<fbxsdk.h>

struct Node
{
	std::string name;

	DirectX::XMVECTOR scaling = { 1,1,1,0 };

	DirectX::XMVECTOR rotation = { 0,0,0,0 };

	DirectX::XMVECTOR translation = { 0,0,0,1 };

	DirectX::XMMATRIX transform;

	DirectX::XMMATRIX globalTransform;

	Node* parent = nullptr;
};

class Model
{
private:
	template<class T> using ComPtr =
		Microsoft::WRL::ComPtr<T>;

	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using TexMetadata = DirectX::TexMetadata;
	using ScratchImage = DirectX::ScratchImage;

	using string = std::string;
	template<class T> using vector = std::vector<T>;

public:
	static const int maxBoneIndices = 4;

	friend class FbxLoader;

	struct VertexPosNormalUvSkin
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT2 uv;
		UINT boneIndex[maxBoneIndices];
		float boneWeight[maxBoneIndices];
	};

	struct Bone
	{
		std::string name;

		DirectX::XMMATRIX invInitialPose;

		FbxCluster* fbxCluster;

		Bone(const std::string& name) {
			this->name = name;
		}
	};

	Node* meshNode = nullptr;

	std::vector<VertexPosNormalUvSkin> vertices;

	std::vector<unsigned short> indices;

	DirectX::XMFLOAT3 ambient = { 1,1,1 };

	DirectX::XMFLOAT3 diffuse = { 1,1,1 };

	DirectX::TexMetadata metadata = {};

	DirectX::ScratchImage scratchimg = {};

private:

	ComPtr<ID3D12Resource> vertBuff;

	ComPtr<ID3D12Resource> indexBuff;

	ComPtr<ID3D12Resource> texbuff;

	D3D12_VERTEX_BUFFER_VIEW vbView = {};

	D3D12_INDEX_BUFFER_VIEW idView = {};

	ComPtr<ID3D12DescriptorHeap> descHeapSRV;

public:
	//バッファー生成
	void CreateBuffers(ID3D12Device* device);
	//描画
	void Draw(ID3D12GraphicsCommandList* cmdList);
	//モデルの変形行列取得
	const XMMATRIX GetModelTransform() { return meshNode->globalTransform; }

	//モデルの逆変換行列取得
	const XMMATRIX GetModelTransformInverse() { return DirectX::XMMatrixInverse(nullptr, meshNode->globalTransform); }

	std::vector<Bone>& GetBones() { return bones; }

	FbxScene* GetFbxScene() { return fbxScene; }

	~Model();

private:
	std::string name;

	std::vector<Node> nodes;

	//ボーン配列
	std::vector<Bone> bones;

	FbxScene* fbxScene = nullptr;
};