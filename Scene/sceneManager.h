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

	//ƒV[ƒ“‚Ì•t‚¯‘Ö‚¦
	void replacementScene();

public:
	primitiveScene* nowscene;

private:
};