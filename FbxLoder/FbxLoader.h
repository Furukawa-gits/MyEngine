#pragma once

#include"../lib/fbx_sdk/include/fbxsdk.h"

#include <d3d12.h>
#include <d3dx12.h>
#include <string>
#include"Model.h"

class FbxLoader
{
private:
	using string = std::string;
public:
	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns>インスタンス</returns>
	static FbxLoader* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="device">デバイス情報</param>
	void Initilize(ID3D12Device* device);

	Model* LoadmodelFromFile(const string& modelName);

	void ParseNodeRecursive(Model* model, FbxNode* fbxnode, Node* parent = nullptr);
	//メッシュ読み取り
	void ParseMesh(Model* model, FbxNode* fbxNode);
	//頂点情報
	void ParseMeshVertices(Model* model, FbxMesh* fbxmesh);
	//面情報
	void ParseMeshFaces(Model* model, FbxMesh* fbxMesh);
	//マテリアル
	void ParseMeshMaterial(Model* model, FbxNode* fbxNode);
	//テクスチャ
	void LoadTexture(Model* model, const std::string& fullpath);
	//ディレクトリ込みのパスからファイル名抽出
	std::string ExtractFileName(const std::string& path);
	//行列変換
	static void ConvertMatrixFromFbx(DirectX::XMMATRIX* dst, FbxAMatrix& src);
	//スキニング情報読み取り
	void ParseSkin(Model* model, FbxMesh* fbxMesh);

	/// <summary>
	/// 後始末
	/// </summary>
	void Finalize();

private:
	// privateなコンストラクタ（シングルトンパターン）
	FbxLoader() = default;
	// privateなデストラクタ（シングルトンパターン）
	~FbxLoader() = default;
	// コピーコンストラクタを禁止（シングルトンパターン）
	FbxLoader(const FbxLoader& obj) = delete;
	// コピー代入演算子を禁止（シングルトンパターン）
	void operator=(const FbxLoader& obj) = delete;

	ID3D12Device* device = nullptr;
	FbxManager* fbxManager = nullptr;
	FbxImporter* fbxImporter = nullptr;

public:
	static const string baseDirectory;
	static const string defaultTextureFileName;
};