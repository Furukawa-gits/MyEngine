#pragma once

#include "Model.h"
#include "FbxLoader.h"
#include "../camera/Camera.h"

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include <string>

class Object3d_FBX
{
protected:
	template<class T> using ComPtr =
		Microsoft::WRL::ComPtr<T>;

	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	static const int MAX_BONES = 32;

	struct ConstBufferDataTransform
	{
		XMMATRIX viewproj;
		XMMATRIX world;
		XMFLOAT3 cameraPos;
	};

	struct ConstBufferDataSkin
	{
		XMMATRIX bones[MAX_BONES];
	};

public:
	static void SetDevice(ID3D12Device* device) { Object3d_FBX::device = device; }
	static void SetCamera(Camera* camera) { Object3d_FBX::camera = camera; }

	void Initialize();

	static void CreateGraphicsPipeline();

	void Update();

	void SetModel(Model* model) { this->model = model; }

	void SetPosition(XMFLOAT3 pos) { this->position = pos; }

	void SetRotation(XMFLOAT3 rot) { this->rotation = rot; }

	void SetScale(XMFLOAT3 Scale) { this->scale = scale; }

	void Draw(ID3D12GraphicsCommandList* cmdList);

	void PlayAnimation();

protected:
	ComPtr<ID3D12Resource> constBufferTransform;

	ComPtr<ID3D12Resource> constBufferSkin;

private:
	static ID3D12Device* device;

	static Camera* camera;

	static ComPtr<ID3D12RootSignature> rootsignature;

	static ComPtr<ID3D12PipelineState> pipelinestate;

	Camera* respectiveCamera = nullptr;

	XMFLOAT3 scale = { 1,1,1 };

	XMFLOAT3 rotation = { 0,0,0 };

	XMFLOAT3 position = { 0,0,0 };

	XMMATRIX matWorld;

	Model* model = nullptr;

	FbxTime frameTime;

	FbxTime starttime;

	FbxTime endTime;

	FbxTime currentTime;

	bool isPlay = false;
};