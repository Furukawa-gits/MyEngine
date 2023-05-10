#include "titleScene.h"

titleScene::titleScene()
{
	//リソース読み込み
	loadResources();

	//パラメータのセット
	setParameter();

	thisType = gameSceneType::title;
}

titleScene::~titleScene()
{

}

void titleScene::loadResources()
{
	//背景
	titleBack.size = { 1280,720 };
	titleBack.generateSprite("black_color.png");

	//タイトル
	titleIconDrag.anchorpoint = { 0.5f,0.5f };
	titleIconDrag.size = { 250,110 };
	titleIconDrag.position = { 0,200,0 };
	titleIconDrag.generateSprite("titleDrag.png");

	titleIconShoot.anchorpoint = { 0.5f,0.5f };
	titleIconShoot.size = { 300,100 };
	titleIconShoot.position = { 0,310,0 };
	titleIconShoot.generateSprite("titleShoot.png");

	titlePlayer.anchorpoint = { 0.5f,0.5f };
	titlePlayer.size = { 410,310 };
	titlePlayer.position = { 600,300,0 };
	titlePlayer.generateSprite("titlePlayer.png");

	titleWhiteBack.size = { 1280,720 };
	titleWhiteBack.generateSprite("white1x1.png");

	//スタートボタン
	startButton.anchorpoint = { 0.5f,0.5f };
	startButton.size = { 430,50 };
	startButton.position = { 640,500,0 };
	startButton.generateSprite("startButton.png");

	//マウスカーソル
	mouseCursur.anchorpoint = { 0.5f,0.5f };
	mouseCursur.size = { 20,20 };
	mouseCursur.generateSprite("Target.png");

	mouseCursurSub.anchorpoint = { 0.5f,0.5f };
	mouseCursurSub.size = { 20,20 };
	mouseCursurSub.generateSprite("Target.png");
}

void titleScene::initialize()
{
	//リソース読み込み
	loadResources();

	//パラメータのセット
	setParameter();
}

void titleScene::setParameter()
{
	//タイトルアニメーション準備
	titleDragEase.set(easingType::easeOut, easingPattern::Quadratic, titleEaseTime, dragEaseStart, dragEaseEnd);
	titleShootEase.set(easingType::easeOut, easingPattern::Quadratic, titleEaseTime, shootEaseStart, shootEaseEnd);
	isTitleAnimation = true;
	titleWhiteBackAlpha = 1.0f;
	titleIconDrag.position.x = dragEaseStart;
	titleIconShoot.position.x = shootEaseStart;
	titleIconDrag.spriteUpdata();
	titleIconShoot.spriteUpdata();
	isPushStart = false;
	isNextScene = false;
}

void titleScene::updata()
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

	titleIconDrag.spriteUpdata();
	titleIconShoot.spriteUpdata();

	titleBack.spriteUpdata();

	if (!titleDragEase.getIsActive() && !titleShootEase.getIsActive() && isTitleAnimation)
	{
		isTitleAnimation = false;
	}

	if (isTitleAnimation)
	{
		titleIconDrag.position.x = titleDragEase.easing();
		titleIconShoot.position.x = titleShootEase.easing();

		return;
	}
	else
	{
		titleIconDrag.position.x = dragEaseEnd;
		titleIconShoot.position.x = shootEaseEnd;
		titleWhiteBackAlpha -= 1.0f / 240.0f;
		titleWhiteBack.color = { 1,1,1,titleWhiteBackAlpha };
	}

	titleWhiteBack.spriteUpdata();
	titlePlayer.spriteUpdata();

	if ((startButton.isSpriteMouseInput() || input->Triger(DIK_SPACE)) && !isPushStart && !isTitleAnimation)
	{
		titleStartButtonEaseY.set(easingType::easeOut, easingPattern::Quadratic, 30, startButton.size.y, 0);
		titleStartButtonEaseX.set(easingType::easeOut, easingPattern::Quadratic, 30, startButton.size.x, startButton.size.x + 30);
		titleWhiteBackAlpha = 0.0f;
		isPushStart = true;
	}

	if (isPushStart)
	{
		startButton.size.y = titleStartButtonEaseY.easing();
		startButton.size.x = titleStartButtonEaseX.easing();
	}

	startButton.spriteUpdata();
	titleBack.spriteUpdata();

	if (isPushStart && !titleStartButtonEaseY.getIsActive())
	{
		isNextScene = true;
	}
}

void titleScene::drawBack()
{
	sample_back->drawSprite(directx->cmdList.Get());
}

void titleScene::draw3D()
{
}

void titleScene::draw2D()
{
	titleBack.drawSprite(directx->cmdList.Get());

	if (!isTitleAnimation)
	{
		titleWhiteBack.drawSprite(directx->cmdList.Get());
		titlePlayer.drawSprite(directx->cmdList.Get());
		startButton.drawSprite(directx->cmdList.Get());
	}

	titleIconDrag.drawSprite(directx->cmdList.Get());
	titleIconShoot.drawSprite(directx->cmdList.Get());

	mouseCursurSub.drawSprite(directx->cmdList.Get());
	mouseCursur.drawSprite(directx->cmdList.Get());
}
