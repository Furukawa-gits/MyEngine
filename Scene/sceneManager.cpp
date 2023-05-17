#include "sceneManager.h"

sceneManager::sceneManager()
{
	//ŠeƒV[ƒ“‚Ì¶¬
	title = new titleScene();
	select = new selectScene();
	play = new playScene();
	result = new resultScene();

	nowscene = title;

	nowscene->setParameter();
}

sceneManager::~sceneManager()
{
	delete(nowscene);
	delete(title);
	delete(select);
	delete(play);
	delete(result);
}

void sceneManager::replacementScene()
{
	if (!nowscene->isNextScene)
	{
		return;
	}

	switch (nowscene->thisType)
	{
	case gameSceneType::title:
		nowscene = select;
		nowscene->setParameter();
		break;
	case gameSceneType::select:
		if (primitiveScene::stageNum == -1)
		{
			nowscene = title;
			nowscene->setParameter();
		}
		else
		{
			nowscene = play;
			nowscene->setParameter();
		}
		break;
	case gameSceneType::play:
		nowscene = result;
		nowscene->setParameter();
		break;
	case gameSceneType::result:
		if (!primitiveScene::isSelectOrTitle)
		{
			nowscene = title;
			nowscene->setParameter();
		}
		else
		{
			nowscene = select;
			nowscene->setParameter();
		}
		break;
	default:
		break;
	}
}
