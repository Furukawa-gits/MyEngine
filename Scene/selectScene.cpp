#include "selectScene.h"

void selectScene::loadResources()
{
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
	stage[0].position.x = 640 - 300;
	isMoveStageIcon = false;
	isPushStart = false;
	stageNum = 0;
	selectIconSizeX = 350;
}

void selectScene::updata()
{
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
			return;
		}
		//チュートリアル開始
		else if (stageNum == 0)
		{
			return;
		}
		else
		{
			//ステージ読み込み
			isLoadStage = loadStage();
		}

		if (isLoadStage)
		{
			return;
		}
	}
}

void selectScene::draw3D()
{
}

void selectScene::draw2D()
{
}

bool selectScene::loadStage()
{

}