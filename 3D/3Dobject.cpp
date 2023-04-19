#include"3Dobject.h"

/// <summary>
/// 静的メンバ変数の実体
/// </summary>
Camera* object3Dobj::camera = nullptr;
directX* object3Dobj::directx = nullptr;
ObjectCommon* object3Dobj::objectcommon = nullptr;

object3Dobj::object3Dobj()
{
}

void object3Dobj::staticInit(Camera* camera, directX* directx)
{
	object3Dobj::camera = camera;
	object3Dobj::directx = directx;
}

void object3Dobj::setStaticData(Camera* camera, directX* directx, ObjectCommon* common)
{
	object3Dobj::camera = camera;
	object3Dobj::directx = directx;
	object3Dobj::objectcommon = common;
}

//3Dオブジェクト初期化
void object3Dobj::init3d(int index)
{
	HRESULT result;

	//ヒープ設定
	D3D12_HEAP_PROPERTIES cbheapprop{};
	cbheapprop.Type = D3D12_HEAP_TYPE_UPLOAD;

	//リソース設定
	D3D12_RESOURCE_DESC cbresdescB0{};
	cbresdescB0.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbresdescB0.Width = (sizeof(ConstBufferDataB0) + 0xff) & ~0xff;
	cbresdescB0.Height = 1;
	cbresdescB0.DepthOrArraySize = 1;
	cbresdescB0.MipLevels = 1;
	cbresdescB0.SampleDesc.Count = 1;
	cbresdescB0.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//定数バッファの生成(B0)
	result = directx->dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&cbresdescB0,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constbuffB0));

	//リソース設定
	D3D12_RESOURCE_DESC cbresdescB1{};
	cbresdescB1.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbresdescB1.Width = (sizeof(ConstBufferDataB1) + 0xff) & ~0xff;
	cbresdescB1.Height = 1;
	cbresdescB1.DepthOrArraySize = 1;
	cbresdescB1.MipLevels = 1;
	cbresdescB1.SampleDesc.Count = 1;
	cbresdescB1.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//定数バッファの生成
	result = directx->dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&cbresdescB1,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constbuffB1));


	//デスクリプタ１つ分のサイズ
	UINT descHandleIncrementSize =
		directx->dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	//CBVのCPUデスクリプタハンドルを取得
	cpuDescHandleCBV = objectcommon->basicDescHeap->GetCPUDescriptorHandleForHeapStart();
	cpuDescHandleCBV.ptr += index * descHandleIncrementSize;

	//CBVのGPUデスクリプタハンドルを取得
	gpuDescHandleCBV = objectcommon->basicDescHeap->GetGPUDescriptorHandleForHeapStart();
	gpuDescHandleCBV.ptr += index * descHandleIncrementSize;

	//定数バッファビューの作成
	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc{};
	cbvDesc.BufferLocation = constbuffB0->GetGPUVirtualAddress();
	cbvDesc.SizeInBytes = (UINT)constbuffB0->GetDesc().Width;
	directx->dev->CreateConstantBufferView(&cbvDesc, cpuDescHandleCBV);
}

//3Dオブジェクト更新処理
void object3Dobj::update3d(Viewes& view, XMFLOAT4 color)
{
	XMMATRIX matscale, matrot, mattrans;

	//スケール、回転、平行移動行列の計算
	matscale = XMMatrixScaling(scale.x, scale.y, scale.z);
	matrot = XMMatrixIdentity();
	matrot *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));
	matrot *= XMMatrixRotationX(XMConvertToRadians(rotation.x));
	matrot *= XMMatrixRotationY(XMConvertToRadians(rotation.y));
	mattrans = XMMatrixTranslation(
		position.x, position.y, position.z);

	matWorld = XMMatrixIdentity();
	matWorld *= matscale;
	matWorld *= matrot;
	matWorld *= mattrans;

	if (parent != nullptr)
	{
		matWorld *= parent->matWorld;
	}

	//定数バッファへのデータ転送
	ConstBufferDataB0* constMapB0 = nullptr;
	ConstBufferDataB1* constMapB1 = nullptr;

	if (SUCCEEDED(constbuffB0->Map(0, nullptr, (void**)&constMapB0)))
	{
		constMapB0->mat = matWorld * camera->GetViewMatrix() * camera->GetProjectionMatrix();
		constbuffB0->Unmap(0, nullptr);
	}

	if (SUCCEEDED(constbuffB1->Map(0, nullptr, (void**)&constMapB1)))
	{
		constMapB1->ambient = view.material.ambient;
		constMapB1->diffuse = view.material.diffuse;
		constMapB1->specular = view.material.specular;
		constMapB1->alpha = view.material.alpha;
		constbuffB1->Unmap(0, nullptr);
	}
}

//3Dオブジェクト描画処理
void object3Dobj::Draw(ID3D12DescriptorHeap* TexdescHeap, Viewes& view, D3D12_GPU_DESCRIPTOR_HANDLE gpuDeschandleSRV, indices_num num)
{
	directx->cmdList.Get()->IASetVertexBuffers(0, 1, &view.vbView);

	directx->cmdList.Get()->IASetIndexBuffer(&view.idView);

	ID3D12DescriptorHeap* ppHeaps[] = { TexdescHeap };
	directx->cmdList.Get()->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	directx->cmdList.Get()->SetGraphicsRootConstantBufferView(0, constbuffB0->GetGPUVirtualAddress());

	directx->cmdList.Get()->SetGraphicsRootDescriptorTable(1, gpuDeschandleSRV);

	if (num == cube)
	{
		directx->cmdList.Get()->DrawIndexedInstanced(36, 1, 0, 0, 0);
	}
	else if (num == tri)
	{
		directx->cmdList.Get()->DrawIndexedInstanced(18, 1, 0, 0, 0);
	}
	else if (num == plane)
	{
		directx->cmdList.Get()->DrawIndexedInstanced(6, 1, 0, 0, 0);
	}
	else
	{
		return;
	}
}

//モデル描画(マテリアル無し)
void object3Dobj::drawModel(ID3D12DescriptorHeap* TexdescHeap, Viewes& view, D3D12_GPU_DESCRIPTOR_HANDLE gpuDeschandleSRV)
{
	directx->cmdList.Get()->IASetVertexBuffers(0, 1, &view.vbView);

	directx->cmdList.Get()->IASetIndexBuffer(&view.idView);

	ID3D12DescriptorHeap* ppHeaps[] = { TexdescHeap };
	directx->cmdList.Get()->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	directx->cmdList.Get()->SetGraphicsRootConstantBufferView(0, constbuffB0->GetGPUVirtualAddress());
	directx->cmdList.Get()->SetGraphicsRootConstantBufferView(1, constbuffB1->GetGPUVirtualAddress());

	directx->cmdList.Get()->SetGraphicsRootDescriptorTable(2, gpuDeschandleSRV);

	directx->cmdList.Get()->DrawIndexedInstanced((UINT)view.modelindices.size(), 1, 0, 0, 0);
}

//モデル描画(マテリアルあり)
void object3Dobj::drawModelOnMaterial(Viewes& view)
{
	directx->cmdList.Get()->IASetVertexBuffers(0, 1, &view.vbView);

	directx->cmdList.Get()->IASetIndexBuffer(&view.idView);

	ID3D12DescriptorHeap* ppHeaps[] = { objectcommon->materialDescHeap.Get() };
	directx->cmdList.Get()->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	directx->cmdList.Get()->SetGraphicsRootConstantBufferView(0, constbuffB0->GetGPUVirtualAddress());
	directx->cmdList.Get()->SetGraphicsRootConstantBufferView(1, constbuffB1->GetGPUVirtualAddress());

	directx->cmdList.Get()->SetGraphicsRootDescriptorTable(2, CD3DX12_GPU_DESCRIPTOR_HANDLE(
		objectcommon->materialDescHeap->GetGPUDescriptorHandleForHeapStart(),
		view.material.material_texnumber,
		directx->dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));

	directx->cmdList.Get()->DrawIndexedInstanced((UINT)view.modelindices.size(), 1, 0, 0, 0);
}

//ワールド座標からスクリーン座標
XMVECTOR object3Dobj::worldToScreenPos(XMVECTOR World_pos)
{
	float w = (float)1280.0f / 2.0f;
	float h = (float)720.0f / 2.0f;
	XMMATRIX viewport = {
		w, 0, 0, 0,
		0,-h, 0, 0,
		0, 0, 1, 0,
		w, h, 0, 1
	};

	World_pos = XMVector3Transform(World_pos, camera->GetViewMatrix());
	World_pos = XMVector3Transform(World_pos, camera->GetProjectionMatrix());

	XMFLOAT3 temp;
	XMStoreFloat3(&temp, World_pos);

	const XMVECTOR view_vec = XMVectorSet(temp.x / temp.z, temp.y / temp.z, 1.0f, 1.0f);

	return XMVector3Transform(view_vec, viewport);
}

float returnScaler(XMFLOAT3 vec)
{
	return sqrtf(
		vec.x * vec.x +
		vec.y * vec.y +
		vec.z * vec.z
	);
}

XMFLOAT3 normalized(XMFLOAT3 vec)
{
	float scl = returnScaler(vec);

	return { vec.x / scl,vec.y / scl,vec.z / scl };
}
