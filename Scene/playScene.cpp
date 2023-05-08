#include "playScene.h"

playScene::playScene()
{
	//リソース読み込み
	loadResources();

	//パラメータのセット
	setParameter();
}

void playScene::loadResources()
{
#pragma region	//チュートリアル用のテキスト
	moveText.anchorpoint = { 0.5f,0.5f };
	moveText.size = { 375,75 };
	moveText.position = { 640,600,0 };
	moveText.generateSprite("moveText.png");

	boostText.anchorpoint = { 0.5f,0.5f };
	boostText.size = { 398,75 };
	boostText.position = { 640,600,0 };
	boostText.generateSprite("boostText.png");

	shotText.anchorpoint = { 0.5f,0.5f };
	shotText.size = { 300,75 };
	shotText.position = { 640,600,0 };
	shotText.generateSprite("shotText.png");

	missileText.anchorpoint = { 0.5f,0.5f };
	missileText.size = { 593,94 };
	missileText.position = { 640,600,0 };
	missileText.generateSprite("missileText.png");

	shootingText.anchorpoint = { 0.5f,0.5f };
	shootingText.size = { 398,75 };
	shootingText.position = { 640,600,0 };
	shootingText.generateSprite("shootingText.png");
#pragma endregion //チュートリアル用のテキスト

#pragma region	//カウントダウンアイコン
	countDownSprite[0].anchorpoint = { 0.5f,0.5f };
	countDownSprite[0].size = { 100,100 };
	countDownSprite[0].position = { 640,360,0 };
	countDownSprite[0].generateSprite("count3.png");
	countDownSprite[0].spriteUpdata();

	countDownSprite[1].anchorpoint = { 0.5f,0.5f };
	countDownSprite[1].size = { 100,100 };
	countDownSprite[1].position = { 640,360,0 };
	countDownSprite[1].generateSprite("count2.png");
	countDownSprite[1].spriteUpdata();

	countDownSprite[2].anchorpoint = { 0.5f,0.5f };
	countDownSprite[2].size = { 100,100 };
	countDownSprite[2].position = { 640,360,0 };
	countDownSprite[2].generateSprite("count1.png");
	countDownSprite[2].spriteUpdata();
#pragma endregion //カウントダウンアイコン

	//スタートテキスト
	playStart.anchorpoint = { 0.5f,0.5f };
	playStart.size = { 500,125 };
	playStart.position = { 640,360,0 };
	playStart.generateSprite("playstart.png");

	//高度メーター
	heightGauge.anchorpoint = { 0.5f,0.5f };
	heightGauge.size = { 40,200 };
	heightGauge.position = { Enemy::miniMapPosition.x + 120,Enemy::miniMapPosition.y,0 };
	heightGauge.generateSprite("playerheightbar.png");

	//プレイヤーの高度
	playerHeight.anchorpoint = { 0.5f,0.5f };
	playerHeight.size = { 32,5 };
	playerHeight.generateSprite("playerHPGauge.png");
	playerHeightIcon.anchorpoint = { 0.0f,0.5f };
	playerHeightIcon.size = { 44,17 };
	playerHeightIcon.generateSprite("playerHeightIcon.png");

	//天球と地面のモデル
	Model* sky = FbxLoader::GetInstance()->LoadmodelFromFile("skySphere");
	Model* ground = FbxLoader::GetInstance()->LoadmodelFromFile("floar");
	SkyModel = std::make_unique<Model>();
	SkyModel.reset(sky);
	groundModel = std::make_unique<Model>();
	groundModel.reset(ground);

	//天球と地面のオブジェクト初期化
	skySphere = std::make_unique<object3dFBX>();
	skySphere->initialize();
	skySphere->SetModel(SkyModel.get());
	skySphere->SetScale({ 8.0f,8.0f,8.0f });
	groundPlane = std::make_unique<object3dFBX>();
	groundPlane->initialize();
	groundPlane->SetModel(groundModel.get());
	groundPlane->SetPosition(playerPointer->groundPosition);
	groundPlane->SetScale({ 0.5f,0.5f,0.5f });
}

void playScene::initialize()
{
	loadResources();

	setParameter();
}

void playScene::setParameter()
{
	if (isTutorial)
	{
		//プレイヤーのリセット
		playerPointer->reset();
		playerPointer->cameraMoveCount = 0;
		playerPointer->boostCount = 0;
		playerPointer->normalShotCount = 0;
		playerPointer->missileCount = 0;
		playerPointer->isBoostTutorial = false;
		playerPointer->isNormalShot = false;
		playerPointer->isHomingMissile = false;
		isMoveText = true;
		isBoostText = false;
		isShotText = false;
		isMissileText = false;
		isShootingText = false;
		isBoss = false;
		//スタートのカウントダウンを設定
		countDownEase.set(easingType::easeOut, easingPattern::Quintic, countDownTime, 450, 0);
		countDownSprite[0].rotation = 0;
		countDownSprite[1].rotation = 0;
		countDownSprite[2].rotation = 0;
		countDownNum = 0;
		isCountDown = true;
		isStartIcon = false;

		nowStageLevel = 1;
		playerPointer->updata();
		object3dFBX::SetCamera(playerPointer->followCamera);
	}
	else
	{
		//スタートのカウントダウンを設定
		countDownEase.set(easingType::easeOut, easingPattern::Quintic, countDownTime, 450, 0);
		countDownSprite[0].rotation = 0;
		countDownSprite[1].rotation = 0;
		countDownSprite[2].rotation = 0;
		countDownNum = 0;
		isCountDown = true;
		isStartIcon = false;

		nowStageLevel = 1;

		playerPointer->updata();
		object3dFBX::SetCamera(playerPointer->followCamera);
	}
}

void playScene::updata()
{
	if (input->Triger(DIK_SPACE))
	{
		int test = 0;
	}

	//カウントダウン
	countDown();

	//敵に共通して必要なプレイヤーの情報を渡す
	Enemy::staticUpdata(playerPointer->getPlayerPos(), playerPointer->getPlayerFront(), playerPointer->isAlive);

	if (!isCountDown && !isStartIcon)
	{
		//ゲーム時間カウント
		totalPlayFlameCount++;
	}

	//パーティクルの共通カメラを設定
	SingleParticle::setCamera(playerPointer->followCamera);

	//スカイドーム更新
	skySphere->setRotMatrix(0, 0, skyShpereRotY);
	skySphere->updata();

	//地面更新
	groundPlane->updata();

	//高度メーター更新
	//プレイヤーのy座標に補正をかけてpositionに代入
	float playerH = playerPointer->getPlayerPos().y - playerPointer->groundPosition.y;

	//スプライトの座標系に変更＆メーターの上がり具合を調整
	playerH = -(playerH * 0.3333f);

	//実際に表示する座標に変更
	playerH = playerH + heightGauge.position.y + (heightGauge.size.y / 2);

	float underLimit = heightGauge.position.y + (heightGauge.size.y / 2) - 5;
	float upperLimit = heightGauge.position.y - (heightGauge.size.y / 2) + 5;

	if (playerH < upperLimit)
	{
		playerH = upperLimit;
	}
	else if (playerH > underLimit)
	{
		playerH = underLimit;
	}

	playerHeight.position = { heightGauge.position.x,playerH,0 };
	playerHeightIcon.position = { playerHeight.position.x + 20,playerH - 2,0 };

	heightGauge.spriteUpdata();
	playerHeight.spriteUpdata();
	playerHeightIcon.spriteUpdata();

	//チュートリアル
	if (stageNum == 0)
	{
		tutorial();
		return;
	}

	if (input->Triger(DIK_LSHIFT))
	{
		playerPointer->playerHP = 0;
	}

	//プレイヤー更新
	playerPointer->updata();

	//エネミー更新
	enemyList.remove_if([](std::unique_ptr<Enemy>& newenemy)
		{
			return newenemy->isDraw == false;
		});

	for (std::unique_ptr<Enemy>& newenemy : enemyList)
	{
		newenemy->updata();
	}

	//ホーミング弾発射
	if (input->Mouse_LeftRelease() && !isCountDown)
	{
		for (int i = 0; i < targetnum; i++)
		{
			for (std::unique_ptr<Enemy>& newenemy : enemyList)
			{
				if (newenemy->isTargetSet && !newenemy->isSetMissile)
				{
					playerPointer->addMissile(newenemy.get());

					newenemy->isSetMissile = true;

					break;
				}
			}
		}

		if (normalBoss->isTargetSet && !normalBoss->isSetMissile)
		{
			playerPointer->addMissile(normalBoss.get());

			normalBoss->isSetMissile = true;
		}

		if (UniteBoss->isTargetSet && !UniteBoss->isSetMissile)
		{
			playerPointer->addMissile(UniteBoss.get());

			UniteBoss->isSetMissile = true;
		}

		for (int i = 0; i < targetnum; i++)
		{
			for (std::unique_ptr<uniteParts>& newparts : UniteBoss->partsList)
			{
				if (newparts->isTargetSet && !newparts->isSetMissile)
				{
					playerPointer->addMissile(newparts.get());

					newparts->isSetMissile = true;

					break;
				}
			}
		}

		targetnum = 0;
	}

	//当たり判定
	checkHitEverything();

	//死んでいる雑魚敵をカウント
	int count = 0;
	for (std::unique_ptr<Enemy>& newenemy : enemyList)
	{
		if (!newenemy->isDraw)
		{
			count++;
		}
	}

	//すべての雑魚敵が死んでいたら次のウェーブ
	if (enemyList.size() <= 0 && nowStageLevel < maxStageLevel)
	{
		nowStageLevel++;

		playerWaveIcon->position.y += nextWaveDis;

		//最後のウェーブならボス戦
		if (nowStageLevel == maxStageLevel && !isBoss)
		{
			if (stageNum < 3)
			{
				//ボス出現
				normalBoss->bossSet({ 0,5,0 });
				isBoss = true;
			}
			else
			{
				//ユニットボス出現
				UniteBoss->uniteBossSet();
				isBoss = true;
			}
		}
		//でなければ次の敵軍
		else
		{
			//次ウェーブ
			int enemySpawnNum = (rand() % 4) + 3 + stageNum;	//敵出現数
			int nextType = (rand() % 4) + 1;				//敵の種類

			for (int i = 0; i < enemySpawnNum; i++)
			{
				//敵　リスト
				std::unique_ptr<Enemy> newenemy = std::make_unique<Enemy>();
				newenemy->init(enemyPattern::shot);
				newenemy->set({
				(float)(rand() % 50 - 25),
				(float)(rand() % 30 + 15),
				(float)(rand() % 50 - 25) });

				newenemy->changePattern((enemyPattern)nextType);

				enemyList.push_back(std::move(newenemy));
			}
		}
	}

	if (isBoss)
	{
		if (stageNum < 3)
		{
			//ボス更新
			normalBoss->bossUpdate(playerPointer.get());
		}
		else
		{
			//ユニットボス更新
			UniteBoss->uniteBossUpdata();
		}
	}

	//ウェーブUIの更新
	enemyWaveBar->spriteUpdata();

	for (std::unique_ptr<SingleSprite>& newsprite : enemyWaveIcons)
	{
		newsprite->spriteUpdata();
	}

	playerWaveIcon->spriteUpdata();

	//ボスを倒したorプレイヤーが死んだらリザルト
	if (isBoss && (!normalBoss->isDraw) && (!UniteBoss->isDraw))
	{
		isClearOrOver = true;
		isNextScene = true;
	}

	if (!playerPointer->isAlive && !playerPointer->isOverStaging && playerPointer->playerHP <= 0)
	{
		isClearOrOver = false;
		isNextScene = true;
	}
}

void playScene::draw3D()
{
	groundPlane->Draw(directx->cmdList.Get());
	skySphere->Draw(directx->cmdList.Get());

	//プレイヤー描画
	playerPointer->draw3D(directx);

	//敵
	for (std::unique_ptr<Enemy>& newenemy : enemyList)
	{
		newenemy->draw3D(directx);
	}

	//ボス描画
	normalBoss->bossDraw3D(directx);

	UniteBoss->uniteBossDraw3d(directx);
}

void playScene::draw2D()
{
	if (stageNum > 0)
	{
		playerPointer->draw2D(directx, targetnum);
	}

	for (std::unique_ptr<Enemy>& newenemy : enemyList)
	{
		newenemy->draw2D(directx);
	}

	//ボス描画(2d)
	normalBoss->bossDraw2D(directx);

	UniteBoss->uniteBossDraw2d(directx);

	if (isCountDown)
	{
		countDownSprite[countDownNum].drawSprite(directx->cmdList.Get());
	}

	if (isStartIcon)
	{
		playStart.drawSprite(directx->cmdList.Get());
	}

	//ウェーブ描画
	drawNowWave();

	//ミニマップ＆高度メーター描画
	drawPositionUI();

	//チュートリアルテキストの都合上、プレイヤーの描画順は分けている
	if (isTutorial)
	{
		if (isMoveText)
		{
			moveText.drawSprite(directx->cmdList.Get());
		}
		else if (isBoostText)
		{
			boostText.drawSprite(directx->cmdList.Get());
		}
		else if (isShotText && !playerPointer->isStop)
		{
			shotText.drawSprite(directx->cmdList.Get());
		}
		else if (isMissileText)
		{
			missileText.drawSprite(directx->cmdList.Get());
		}
		else if (isShootingText)
		{
			shootingText.drawSprite(directx->cmdList.Get());
		}

		playerPointer->draw2D(directx, targetnum);
	}
}

void playScene::drawNowWave()
{
	if (stageNum <= 0)
	{
		return;
	}

	if (playerPointer->isStop)
	{
		return;
	}

	enemyWaveBar->drawSprite(directx->cmdList.Get());

	for (std::unique_ptr<SingleSprite>& newsprite : enemyWaveIcons)
	{
		newsprite->drawSprite(directx->cmdList.Get());
	}

	playerWaveIcon->drawSprite(directx->cmdList.Get());
}

void playScene::drawPositionUI()
{
	if (playerPointer->isStop)
	{
		return;
	}

	//高度メーター
	heightGauge.drawSprite(directx->cmdList.Get());

	for (std::unique_ptr<Enemy>& newenemy : enemyList)
	{
		newenemy->drawMiniMapIcon(directx);
	}

	normalBoss->drawMiniMapIcon(directx);

	UniteBoss->drawMiniMapIcon(directx);

	playerHeight.drawSprite(directx->cmdList.Get());
	playerHeightIcon.drawSprite(directx->cmdList.Get());
}

void playScene::checkHitPlayerTarget()
{
	//通常の敵
	checkHitManager::checkRockonEnemys(playerPointer.get(), &enemyList, targetnum);

	//ボス
	checkHitManager::checkRockonEnemy(playerPointer.get(), normalBoss.get(), targetnum);

	checkHitManager::checkRockonEnemy(playerPointer.get(), UniteBoss.get(), targetnum);

	for (std::unique_ptr<uniteParts>& newparts : UniteBoss->partsList)
	{
		checkHitManager::checkRockonEnemy(playerPointer.get(), newparts.get(), targetnum);
	}
}

void playScene::checkHitEverything()
{
	checkHitManager::checkMissilesEnemy(&playerPointer->missilesList);

	for (std::unique_ptr<Enemy>& newenemy : enemyList)
	{
		checkHitManager::checkPlayerEnemy(playerPointer.get(), newenemy.get());
		checkHitManager::checkPlayerEnemyRampages(playerPointer.get(), newenemy.get());
		checkHitManager::checkBulletsEnemyRampage(&playerPointer->bulletsList, newenemy.get());
	}

	//敵がロックオンされているかどうか
	checkHitPlayerTarget();

	//プレイヤーの通常弾当たり判定
	checkHitManager::checkBulletsEnemys(&playerPointer->bulletsList, &enemyList);

	//通常弾とボスの当たり判定
	checkHitManager::checkBulletsEnemy(&playerPointer->bulletsList, normalBoss.get());

	//通常弾とユニットボス本体の当たり判定
	checkHitManager::checkBulletsEnemy(&playerPointer->bulletsList, UniteBoss.get());

	//パーツの当たり判定
	for (std::unique_ptr<uniteParts>& newparts : UniteBoss->partsList)
	{
		checkHitManager::checkBulletsEnemy(&playerPointer->bulletsList, newparts.get());
	}

	//プレイヤーとボスの当たり判定
	if (!normalBoss->getIsAppear() && stageNum < 3)
	{
		checkHitManager::checkPlayerEnemy(playerPointer.get(), normalBoss.get());
		checkHitManager::checkPlayerEnemyRampages(playerPointer.get(), normalBoss.get());
		checkHitManager::checkBulletsEnemyRampage(&playerPointer->bulletsList, normalBoss.get());
	}

	//ユニットボス本体との当たり判定
	if (!UniteBoss->getIsAppear() && stageNum == 3)
	{
		checkHitManager::checkPlayerEnemy(playerPointer.get(), UniteBoss.get());

		for (std::unique_ptr<uniteParts>& newparts : UniteBoss->partsList)
		{
			checkHitManager::checkPlayerEnemyRampages(playerPointer.get(), newparts.get());
			checkHitManager::checkBulletsEnemyRampage(&playerPointer->bulletsList, newparts.get());
		}
	}
}

void playScene::countDown()
{
	if (!isCountDown && !isStartIcon)
	{
		return;
	}

	if (isCountDown)
	{
		float countDownSize = countDownEase.easing();
		countDownSprite[countDownNum].size = { countDownSize,countDownSize };
		countDownSprite[countDownNum].rotation -= 4;
		countDownSprite[countDownNum].position = { 640,360,0 };
		countDownSprite[countDownNum].spriteUpdata();

		if (!countDownEase.getIsActive())
		{
			if (countDownNum + 1 < 3)
			{
				countDownEase.set(easingType::easeOut, easingPattern::Quintic, countDownTime, 450, 0);
				countDownNum++;
			}
			else
			{
				isStartIcon = true;
				startIconTime = 40;
				isCountDown = false;
			}
		}
	}

	if (isStartIcon)
	{
		startIconTime--;
		playStart.spriteUpdata();

		if (startIconTime <= 0)
		{
			isStartIcon = false;
		}
	}

	//スタートのカウントダウン演出が終わったら動ける
	if (isCountDown || isStartIcon)
	{
		playerPointer->isStop = true;

		//敵
		for (std::unique_ptr<Enemy>& newenemy : enemyList)
		{
			newenemy->isStop = true;
		}

		normalBoss->isStop = true;

		UniteBoss->isStop = true;

		return;
	}
	else
	{
		playerPointer->isStop = false;

		//敵
		for (std::unique_ptr<Enemy>& newenemy : enemyList)
		{
			newenemy->isStop = false;
		}

		normalBoss->isStop = false;

		UniteBoss->isStop = false;
	}

	return;
}

void playScene::tutorial()
{
	playerPointer->updata();

	//一定量カメラを動かしたら敵出現
	if (playerPointer->cameraMoveCount >= 250 && isMoveText)
	{
		isMoveText = false;
		isBoostText = true;

		playerPointer->isBoostTutorial = true;
	}

	//3回ブーストしたら敵出現
	if (!playerPointer->isBoost && playerPointer->boostCount > 2 && isBoostText)
	{
		for (int i = 0; i < 20; i++)
		{
			//敵　リスト
			std::unique_ptr<Enemy> newenemy = std::make_unique<Enemy>();
			newenemy->init(enemyPattern::tutorial);
			newenemy->set({
			(float)(rand() % 100 - 50),
			(float)(rand() % 60 + 30),
			(float)(rand() % 100 - 50) });

			enemyList.push_back(std::move(newenemy));
		}

		isBoostText = false;
		isShotText = true;

		playerPointer->isNormalShot = true;
	}

	//敵が出現演出途中ならカメラをセットする
	int count = 0;
	for (std::unique_ptr<Enemy>& newenemy : enemyList)
	{
		if (newenemy->isAppear == true)
		{
			count++;
		}
	}
	if (count > 0)
	{
		playerPointer->isStop = true;
		playerPointer->isInvisible = 1;
		stagingCamera = new Camera;
		stagingCamera->SetEye({ 0,0,-30 });
		stagingCamera->SetTarget({ 0,30,0 });
		stagingCamera->Update();
		object3dFBX::SetCamera(stagingCamera);
	}
	else
	{
		playerPointer->isStop = false;
		playerPointer->isInvisible = -1;
		object3dFBX::SetCamera(playerPointer->followCamera);
	}

	//通常弾 -> ミサイル
	if (playerPointer->normalShotCount > 3)
	{
		isShotText = false;
		isMissileText = true;

		playerPointer->isHomingMissile = true;
	}

	//ミサイル -> Lets Shooting!!!
	if (playerPointer->missileCount > 1)
	{
		isMissileText = false;
		isShootingText = true;
	}

	XMFLOAT3 playerTargetPos = playerPointer->getTargetPosition();

	if (playerTargetPos.y < 100)
	{
		playerTargetPos.y = playerTargetPos.y + 40;
	}
	else
	{
		playerTargetPos.y = playerTargetPos.y - 40;
	}

	moveText.position = playerTargetPos;

	shotText.position = playerTargetPos;

	moveText.spriteUpdata();
	boostText.spriteUpdata();
	shotText.spriteUpdata();
	missileText.spriteUpdata();
	shootingText.spriteUpdata();

	//エネミー更新
	enemyList.remove_if([](std::unique_ptr<Enemy>& newenemy)
		{
			return newenemy->isDraw == false;
		});

	for (std::unique_ptr<Enemy>& newenemy : enemyList)
	{
		newenemy->updata();
	}

	//当たり判定
	checkHitEverything();

	//ホーミング弾発射
	if (input->Mouse_LeftRelease() && !isCountDown)
	{
		for (int i = 0; i < targetnum; i++)
		{
			for (std::unique_ptr<Enemy>& newenemy : enemyList)
			{
				if (newenemy->isTargetSet && !newenemy->isSetMissile)
				{
					playerPointer->addMissile(newenemy.get());

					newenemy->isSetMissile = true;

					break;
				}
			}
		}

		if (normalBoss->isTargetSet && !normalBoss->isSetMissile)
		{
			playerPointer->addMissile(normalBoss.get());

			normalBoss->isSetMissile = true;
		}

		targetnum = 0;
	}

	//敵をすべて倒したorプレイヤーが死んだらリザルト
	if (enemyList.size() <= 0 && !isMoveText && isShootingText)
	{
		isClearOrOver = true;
		isNextScene = true;
	}
	if (!playerPointer->isAlive && !playerPointer->isOverStaging && playerPointer->playerHP <= 0)
	{
		isClearOrOver = false;
		isNextScene = true;
	}
}