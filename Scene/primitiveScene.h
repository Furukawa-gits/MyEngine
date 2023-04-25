#pragma once
#include"../Base/WindowGenerate.h"
#include"../Base/DirectX_Base.h"
#include"../2D/Sprite.h"
#include"../Input/dxInput.h"
#include"../Audio/Audio.h"
#include"../2D/Debug_Text.h"
#include"../camera/Camera.h"
#include"../FbxLoder/Object3d_FBX.h"
#include"../camera/FollowCamera.h"

class primitiveScene
{
	static void setStaticData();

	virtual void loadResources();

	virtual void initialize();

	virtual void updata();

	virtual void draw3D();

	virtual void draw2D();

	static std::unique_ptr<directX> directx;
	static std::unique_ptr<dxinput> input;
	static std::unique_ptr<Audio> audio;
};