#pragma once
#include"Camera.h"

class StagingCamera
	:public Camera
{
public:
	StagingCamera();

	void setStagingCamera();

	void updata();
};