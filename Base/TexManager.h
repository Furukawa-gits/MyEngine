#pragma once
#include<DirectXTex.h>
#include<d3dx12.h>
#include<wrl.h>

#pragma comment(lib,"d3d12.lib")

using namespace Microsoft::WRL;

using namespace DirectX;

const int texSRVcount = 512;

class TexManager
{
public:
	//テクスチャ用デスクリプタヒープの生成
	ComPtr<ID3D12DescriptorHeap> descHeap;

	D3D12_DESCRIPTOR_HEAP_DESC descHeapdesc{};

	//テクスチャリソース
	ComPtr<ID3D12Resource> texBuff[texSRVcount];

	//初期化
	void Init(ID3D12Device* dev);

	//テクスチャの読み込み
	void LoadTexture(UINT texnumber, const wchar_t* filename, ID3D12Device* dev);

	//テクスチャ番号でシェーダーリソースビューを検索して返せる
	D3D12_GPU_DESCRIPTOR_HANDLE FindReturnSRV(UINT texnumber, ID3D12Device* dev);
};