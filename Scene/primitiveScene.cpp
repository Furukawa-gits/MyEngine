#include "primitiveScene.h"

directX* primitiveScene::directx = nullptr;
dxinput* primitiveScene::input = nullptr;
Audio* primitiveScene::audio = nullptr;
int primitiveScene::stageNum = 1;
int primitiveScene::nowStageLevel = 1;
int primitiveScene::maxStageLevel = 1;
bool primitiveScene::isTutorial = false;

void primitiveScene::setStaticData(directX* Directx, dxinput* Input, Audio* Audio)
{
	directx = Directx;
	input = Input;
	audio = Audio;
}
