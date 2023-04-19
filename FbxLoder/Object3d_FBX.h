#pragma once

#include "Model.h"
#include "FbxLoader.h"
#include "../camera/Camera.h"
#include "../Light/Light.h"

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include <string>

using namespace Microsoft::WRL;
using namespace DirectX;

enum graphicType
{
	default = 0,
	Simple = 1
};

class object3dFBX
{
protected:
	template<class T> using ComPtr =
		Microsoft::WRL::ComPtr<T>;

	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	static const int maxBones = 32;

	struct ConstBufferDataTransform
	{
		XMMATRIX viewproj;
		XMMATRIX world;
		XMFLOAT3 cameraPos;
		float pad;
		XMFLOAT4 color;
	};

	struct ConstBufferDataSkin
	{
		XMMATRIX bones[maxBones];
	};

public:
	object3dFBX();
	~object3dFBX();

	static void SetDevice(ID3D12Device* device) { object3dFBX::device = device; }
	static void SetCamera(Camera* camera) { object3dFBX::camera = camera; }
	static void setLight(Light* light) { object3dFBX::light = light; }

	void initialize();

	static void CreateGraphicsPipeline();
	static void CreateGraphicsPipelineSimple();

	void updata();

	void SetModel(Model* model) { this->model = model; }

	void SetPosition(XMFLOAT3 pos) { this->position = pos; }

	void SetRotation(XMFLOAT3 rot) { this->rotation = rot; }

	void SetScale(XMFLOAT3 Scale) { this->scale = Scale; }

	void setAngle(float up, float right) {
		this->upAngle = up;
		this->rightAngle = right;
	}

	void setSpeed(float speed) { this->moveSpeed = speed; }

	void setColor(XMFLOAT4 col) { this->color = col; }

	void setParent(object3dFBX* parent) { this->parent = parent; }

	XMFLOAT2 worldToScleen();

	XMFLOAT2 worldToScleenSpecifyPosition(XMFLOAT3 pos);

	XMFLOAT3 screenToWorld(XMFLOAT2 screenPos);

	XMFLOAT3 getPosition() { return position; }

	XMFLOAT3 getRotation() { return rotation; }

	XMFLOAT3 getScale() { return scale; }

	XMVECTOR getUpDirection() { return directionUp; }

	void addMoveFront(XMFLOAT3 move) {
		position.x += move.x;
		position.y += move.y;
		position.z += move.z;
	}

	void addMoveBack(XMFLOAT3 move) {
		position.x -= move.x;
		position.y -= move.y;
		position.z -= move.z;
	}

	void addQRot(XMVECTOR rotQ)
	{
		qRot = rotQ;
	}

	void setRotMatrix(XMMATRIX rotM)
	{
		mRot = rotM;
	}

	void setRotMatrix(float rotZ, float rotX, float rotY)
	{
		mRot *= XMMatrixRotationZ(rotZ);
		mRot *= XMMatrixRotationX(rotX);
		mRot *= XMMatrixRotationY(rotY);
	}

	void SetPipelineSimple(ID3D12GraphicsCommandList* cmdList);

	void reSetPipeline();

	void Draw(ID3D12GraphicsCommandList* cmdList);

	void PlayAnimation();

protected:
	ComPtr<ID3D12Resource> constBufferTransform;

	ComPtr<ID3D12Resource> constBufferSkin;

private:
	//デバイス
	static ID3D12Device* device;
	//カメラ
	static Camera* camera;
	//ライト
	static Light* light;

	//パイプライン(通常)
	static ComPtr<ID3D12RootSignature> rootsignature;
	static ComPtr<ID3D12PipelineState> pipelinestate;

	//パイプライン(単色)
	static ComPtr<ID3D12RootSignature> rootsignatureSimple;
	static ComPtr<ID3D12PipelineState> pipelinestateSimple;

	Camera* respectiveCamera = nullptr;

	XMFLOAT3 scale = { 1,1,1 };

	/// <summary>
	/// ロールピッチヨー回転
	/// </summary>
	/// <param name="x">ピッチ</param>
	/// <param name="y">ヨー</param>
	/// <param name="z">ロール</param>
	XMFLOAT3 rotation = { 0,0,0 };

	XMFLOAT3 position = { 0,0,0 };

	XMVECTOR qRot = XMQuaternionIdentity();

	XMMATRIX mRot = XMMatrixIdentity();

	float moveSpeed = 0.0f;

	XMVECTOR directionFront = { 0,0,1,0 };

	XMVECTOR directionUp = { 0,1,0,0 };

	XMMATRIX matScale, matRot, matTrans;//各変換行列

	float upAngle = 0.0f;

	float rightAngle = 0.0f;

	XMMATRIX matWorld = {};

	Model* model = nullptr;

	FbxTime frameTime;

	FbxTime starttime;

	FbxTime endTime;

	FbxTime currentTime;

	bool isPlay = false;

	XMFLOAT4 color = { 1,1,1,1 };

	bool isSetOtherPipeline = false;

	object3dFBX* parent = nullptr;
};