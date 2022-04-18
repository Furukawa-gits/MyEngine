#pragma once
#include<string>
#include<DirectXMath.h>
#include <vector>
#include <DirectXTex.h>

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
public:
	friend class FbxLoader;

	struct VertexPosNormalUv
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT2 uv;
	};

	Node* mesNode = nullptr;

	std::vector<VertexPosNormalUv> vertices;

	std::vector<unsigned short> indices;

	DirectX::XMFLOAT3 ambient = { 1,1,1 };

	DirectX::XMFLOAT3 diffuse = { 1,1,1 };

	DirectX::TexMetadata metadata = {};

	DirectX::ScratchImage scratchimg = {};

private:
	std::string name;

	std::vector<Node> nodes;
};