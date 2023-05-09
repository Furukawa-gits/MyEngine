#include "sceneManager.h"

sceneManager::sceneManager()
{
	//ŠeƒV[ƒ“‚Ì¶¬
	title = new titleScene();
	select = new selectScene();
	play = new playScene();
	result = new resultScene();

	nowscene = title;
}

void sceneManager::replacementScene()
{
	if (!nowscene->isNextScene)
	{
		return;
	}

	switch (nowscene->thisType)
	{
	case sceneType::title:
		nowscene = select;
		nowscene->setParameter();
		break;
	case sceneType::select:
		nowscene = play;
		nowscene->setParameter();
		break;
	case sceneType::play:
		nowscene = result;
		nowscene->setParameter();
		break;
	case sceneType::result:
		if (primitiveScene::isSelectOrTitle)
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
