#include "sceneManager.h"

sceneManager::sceneManager()
{
	nowscene = new titleScene();

	nowscene->setParameter();
}

sceneManager::~sceneManager()
{
	delete(nowscene);
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
		nowscene = new selectScene();
		nowscene->setParameter();
		break;
	case gameSceneType::select:
		if (primitiveScene::stageNum == -1)
		{
			nowscene = new titleScene();
			nowscene->setParameter();
		}
		else
		{
			nowscene = new playScene();
			nowscene->setParameter();
		}
		break;
	case gameSceneType::play:
		nowscene = new resultScene();
		nowscene->setParameter();
		break;
	case gameSceneType::result:
		if (!primitiveScene::isSelectOrTitle)
		{
			nowscene = new titleScene();
			nowscene->setParameter();
		}
		else
		{
			nowscene = new selectScene();
			nowscene->setParameter();
		}
		break;
	default:
		break;
	}
}
