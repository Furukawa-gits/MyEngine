#pragma once
#include"titleScene.h"
#include"selectScene.h"
#include"playScene.h"
#include"resultScene.h"

class sceneManager
{
public:
	sceneManager();
	~sceneManager();

	//�V�[���̕t���ւ�
	void replacementScene();

public:
	primitiveScene* nowscene;

private:
};