#include "selectScene.h"

selectScene::selectScene()
{
	//リソース読み込み
	loadResources();

	//パラメータのセット
	setParameter();

	thisType = gameSceneType::select;
}

selectScene::~selectScene()
{

}

void selectScene::loadResources()
{
	//背景
	selectBack.size = { 1280,720 };
	selectBack.generateSprite("black_color.png");

#pragma region //ステージアイコン
	stage[0].anchorpoint = { 0.5f,0.5f };
	stage[0].size = { 500,100 };
	stage[0].position = { 0,360,0 };
	stage[0].generateSprite("titleText.png");

	stage[1].anchorpoint = { 0.5f,0.5f };
	stage[1].size = { 128,128 };
	stage[1].position = { 0,360,0 };
	stage[1].generateSprite("count0.png");

	stage[2].anchorpoint = { 0.5f,0.5f };
	stage[2].size = { 128,128 };
	stage[2].position = { 0,360,0 };
	stage[2].generateSprite("count1.png");

	stage[3].anchorpoint = { 0.5f,0.5f };
	stage[3].size = { 128,128 };
	stage[3].position = { 0,360,0 };
	stage[3].generateSprite("count2.png");

	stage[4].anchorpoint = { 0.5f,0.5f };
	stage[4].size = { 128,128 };
	stage[4].position = { 0,360,0 };
	stage[4].generateSprite("count3.png");
#pragma endregion //ステージアイコン

	//セレクト画面矢印
	selects[0].anchorpoint = { 0.5f,0.5f };
	selects[0].size = { 100,100 };
	selects[0].position = { 640 - 400,380,0 };
	selects[0].generateSprite("selectL.png");

	selects[1].anchorpoint = { 0.5f,0.5f };
	selects[1].size = { 100,100 };
	selects[1].position = { 640 + 400,380,0 };
	selects[1].generateSprite("selectR.png");

	//プレイスタートテキスト
	spaceStart.anchorpoint = { 0.5f,0.5f };
	spaceStart.size = { 350,50 };
	spaceStart.position = { 640,530,0 };
	spaceStart.generateSprite("spacestart.png");

	//決定ボタン
	selects[2].anchorpoint = { 0.5f,0.5f };
	selects[2].size = { 150,5 };
	selects[2].position = { 640,420,0 };
	selects[2].generateSprite("selectIcon.png");

	toTutorial.anchorpoint = { 0.5f,0.5f };
	toTutorial.size = { 220,50 };
	toTutorial.position = { 640,470,0 };
	toTutorial.generateSprite("toTutorial.png");

	//マウスカーソル
	mouseCursur.anchorpoint = { 0.5f,0.5f };
	mouseCursur.size = { 20,20 };
	mouseCursur.generateSprite("Target.png");

	mouseCursurSub.anchorpoint = { 0.5f,0.5f };
	mouseCursurSub.size = { 20,20 };
	mouseCursurSub.generateSprite("Target.png");
}

void selectScene::initialize()
{
	//リソース読み込み
	loadResources();

	//パラメータのセット
	setParameter();
}

void selectScene::setParameter()
{
	stage[0].position.x = stageIconOffset.x;
	stage[1].position.x = stage[0].position.x + 300;
	stage[2].position.x = stage[1].position.x + 300;
	stage[3].position.x = stage[2].position.x + 300;
	stage[4].position.x = stage[3].position.x + 300;
	//各スプライト更新
	for (int i = 0; i < 5; i++)
	{
		stage[i].spriteUpdata();
	}
	for (int i = 0; i < 3; i++)
	{
		selects[i].spriteUpdata();
	}
	spaceStart.spriteUpdata();
	toTutorial.spriteUpdata();
	isMoveStageIcon = false;
	isPushStart = false;
	selectIconSizeX = 350;
	isNextScene = false;
}

void selectScene::updata()
{
	//マウス座標更新
	MOUSE_POS = { (float)input->mousePoint.x,(float)input->mousePoint.y,0.0f };

	//マウスカーソル更新
	mouseCursur.position = MOUSE_POS;
	mouseCursur.rotation += 2.0f;
	mouseCursur.spriteUpdata();
	mouseCursurSub.position = MOUSE_POS;
	mouseCursurSub.rotation -= 4.0f;
	mouseCursurSub.spriteUpdata();

	//ステージアイコンが動いていなければセレクトボタン受付
	if (!isMoveStageIcon)
	{
		float iconPos = stage[0].position.x;

		//次のステージ
		if (selects[1].isSpriteMouseInput() && stageNum < maxStageNum)
		{
			stageIconEase.set(easingType::easeOut, easingPattern::Cubic, 20, iconPos, iconPos - 300);
			stageNum++;
			isMoveStageIcon = true;
		}
		//前のステージ
		else if (selects[0].isSpriteMouseInput() && stageNum > -1)
		{
			stageIconEase.set(easingType::easeOut, easingPattern::Quadratic, 20, iconPos, iconPos + 300);
			stageNum--;
			isMoveStageIcon = true;
		}
	}
	else
	{
		stage[0].position.x = stageIconEase.easing();

		if (!stageIconEase.getIsActive())
		{
			isMoveStageIcon = false;
		}
	}

	stage[1].position.x = stage[0].position.x + 300;
	stage[2].position.x = stage[1].position.x + 300;
	stage[3].position.x = stage[2].position.x + 300;
	stage[4].position.x = stage[3].position.x + 300;

	//セレクトアイコン通常拡縮
	if (!isPushStart)
	{
		selectIconSizeX -= 0.5f;

		if (selectIconSizeX <= 300)
		{
			selectIconSizeX = 350;
		}
		selects[2].size = { selectIconSizeX,10 };
	}

	//ステージ選択ムーブ中でなければ決定
	if (input->Triger(DIK_SPACE) && !isMoveStageIcon && !isPushStart)
	{
		playStartButtonEaseY.set(easingType::easeOut, easingPattern::Quadratic, 30, 20, 0);
		playStartButtonEaseX.set(easingType::easeOut, easingPattern::Quadratic, 30, 300, 350);
		isPushStart = true;
		stageIconOffset = stage[0].position;
	}

	//スタートボタン拡縮
	if (isPushStart)
	{
		selects[2].size.y = playStartButtonEaseY.easing();
		selects[2].size.x = playStartButtonEaseX.easing();
	}

	//各スプライト更新
	for (int i = 0; i < 5; i++)
	{
		stage[i].spriteUpdata();
	}

	for (int i = 0; i < 3; i++)
	{
		selects[i].spriteUpdata();
	}

	spaceStart.spriteUpdata();

	toTutorial.spriteUpdata();

	if (isPushStart && !playStartButtonEaseY.getIsActive())
	{
		isPushStart = false;

		//タイトルに戻る
		if (stageNum == -1)
		{
			isNextScene = true;
			return;
		}
		//チュートリアル開始
		else if (stageNum == 0)
		{
			isNextScene = true;
			return;
		}
		else
		{
			//ステージ読み込み
			isLoadStage = loadStage();
		}

		if (isLoadStage)
		{
			isNextScene = true;
			return;
		}

		stageIconOffset = stage[0].position;
	}
}

void selectScene::drawBack()
{
	sample_back->drawSprite(directx->cmdList.Get());
}

void selectScene::draw3D()
{
}

void selectScene::draw2D()
{
	selectBack.drawSprite(directx->cmdList.Get());
	stage[0].drawSprite(directx->cmdList.Get());
	stage[1].drawSprite(directx->cmdList.Get());
	stage[2].drawSprite(directx->cmdList.Get());
	stage[3].drawSprite(directx->cmdList.Get());
	stage[4].drawSprite(directx->cmdList.Get());

	selects[0].drawSprite(directx->cmdList.Get());
	selects[1].drawSprite(directx->cmdList.Get());
	selects[2].drawSprite(directx->cmdList.Get());

	if (!isTutorial && stageNum > 0)
	{
		toTutorial.drawSprite(directx->cmdList.Get());
	}

	spaceStart.drawSprite(directx->cmdList.Get());

	mouseCursurSub.drawSprite(directx->cmdList.Get());
	mouseCursur.drawSprite(directx->cmdList.Get());
}

bool selectScene::loadStage()
{
	if (stageNum < 1)
	{
		return false;
	}

	if (isTutorial == false)
	{
#ifdef _DEBUG
		return false;
		/*playerPointer->isBoostTutorial = true;
		playerPointer->isNormalShot = true;
		playerPointer->isHomingMissile = true;*/
#else
		return false;
#endif

	}

	for (int i = 0; i < stageNum + 4; i++)
	{
		//敵　リスト
		std::unique_ptr<Enemy> newenemy = std::make_unique<Enemy>();
		newenemy->init(enemyPattern::shot);
		newenemy->set({
		(float)(rand() % 50 - 25),
		(float)(rand() % 30 + 15),
		(float)(rand() % 50 - 25) });

		enemyList.push_back(std::move(newenemy));
	}

	//プレイヤーのリセット
	playerPointer->reset();

	if (stageNum == 1)
	{
		maxStageLevel = stageNum + 2;

		//敵
		normalBoss->changePattern(enemyPattern::chase);

		//敵　リスト
		for (std::unique_ptr<Enemy>& newenemy : enemyList)
		{
			newenemy->changePattern(enemyPattern::shot);
		}

		//ボスの設定
		normalBoss->setHitPoint(maxStageLevel + 5);
	}
	else if (stageNum == 2)
	{
		maxStageLevel = stageNum + 2;

		//敵
		normalBoss->changePattern(enemyPattern::rampage);

		//敵　リスト
		for (std::unique_ptr<Enemy>& newenemy : enemyList)
		{
			newenemy->changePattern(enemyPattern::shot);
		}

		//ボスの設定
		normalBoss->setHitPoint(maxStageLevel + 5);
	}
	else if (stageNum == 3)
	{
		maxStageLevel = stageNum + 2;

		//敵　リスト
		for (std::unique_ptr<Enemy>& newenemy : enemyList)
		{
			newenemy->changePattern(enemyPattern::rampage);
		}
	}

	//アイコン同士の距離を計算
	nextWaveDis = totalWaveBarLength / (maxStageLevel - 1);

	enemyWaveIcons.clear();

	playerWaveIcon->position.y = 360 - totalWaveBarLength / 2;

	for (int i = 0; i < maxStageLevel - 1; i++)
	{
		std::unique_ptr<SingleSprite> newicon = std::make_unique<SingleSprite>();
		newicon->anchorpoint = { 0.5f,0.5f };
		newicon->size = { 50,50 };
		newicon->position = { waveBarPosX,(360 - totalWaveBarLength / 2) + i * nextWaveDis,0 };
		newicon->generateSprite("enemyWaveIcon.png");
		newicon->spriteUpdata();
		enemyWaveIcons.push_back(std::move(newicon));
	}

	std::unique_ptr<SingleSprite> newicon = std::make_unique<SingleSprite>();
	newicon->anchorpoint = { 0.5f,0.5f };
	newicon->size = { 100,100 };
	newicon->position = { waveBarPosX,(360 - totalWaveBarLength / 2) + totalWaveBarLength,0 };
	newicon->generateSprite("bossWaveIcon.png");
	newicon->spriteUpdata();
	enemyWaveIcons.push_back(std::move(newicon));

	return true;
}