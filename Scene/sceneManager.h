#pragma once
#include"titleScene.h"
#include"selectScene.h"
#include"playScene.h"
#include"resultScene.h"

class sceneManager
{
public:
	sceneManager();

	//�V�[���̕t���ւ�
	void replacementScene();

public:
	primitiveScene* nowscene;

private:
	titleScene* title;
	selectScene* select;
	playScene* play;
	resultScene* result;
};