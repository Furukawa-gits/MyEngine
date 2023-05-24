#include"Player.h"

dxinput* Player::input = nullptr;
std::unique_ptr<Model> Player::playerModel = std::make_unique<Model>();


Player::Player()
{
}

Player::~Player()
{
	delete(playerObject);
	delete(followCamera);
	bulletsList.clear();
	missilesList.clear();

	bullet::staticDestroy();
	Missile::staticDestroy();
}

void Player::init(dxinput* input, directX* directx)
{
	this->input = input;

	//ターゲットアイコン
	targetFirst.anchorpoint = { 0.5f,0.5f };
	targetFirst.size = { 40,40 };
	targetFirst.generateSprite("Target.png");

	targetSecond.anchorpoint = { 0.5f,0.5f };
	targetSecond.size = { 60,60 };
	targetSecond.generateSprite("Target.png");

	targetThird.anchorpoint = { 0.5f,0.5f };
	targetThird.size = { 80,80 };
	targetThird.generateSprite("Target.png");

	//ロックオンモードのゲージ
	rockonGauge[0].anchorpoint = { 0.5f,0.5f };
	rockonGauge[0].size = { 0,20 };
	rockonGauge[0].position = { 640,420,0 };
	rockonGauge[0].generateSprite("playerHPGauge.png");

	rockonGauge[1].anchorpoint = { 0.5f,0.5f };
	rockonGauge[1].size = { 0,10 };
	rockonGauge[1].position = { 640,420,0 };
	rockonGauge[1].generateSprite("boostGauge.png");

	//ダメージエフェクト
	damage.size = { 1280,720 };
	damage.generateSprite("damage.png");

	//HPゲージ
	HPGaugeBar.size = { 50,20 };
	HPGaugeBar.position = { 20,640,0 };
	HPGaugeBar.generateSprite("playerHPGauge.png");

	//HPバーの警告
	dangarHPGaugeBar.size = { (float)maxHP * 40,20 };
	dangarHPGaugeBar.position = { 20,640,0 };
	dangarHPGaugeBar.generateSprite("bossHPGauge.png");

	//ブーストゲージ
	boostGaugeBar.size = { 50,20 };
	boostGaugeBar.position = { 20,590,0 };
	boostGaugeBar.generateSprite("boostGauge.png");

	//上記ゲージのフレーム
	gaugeFrame.size = { 500,120 };
	gaugeFrame.position = { 0,550,0 };
	gaugeFrame.generateSprite("gaugeFrame.png");

	//エリア外警告
	outAreaCaution.anchorpoint = { 0.5f,0.5f };
	outAreaCaution.size = { 645,150 };
	outAreaCaution.position = { 640,360,0 };
	outAreaCaution.generateSprite("outAreaCaution.png");

	//ミニマップアイコン
	miniMapPlayer.anchorpoint = { 0.5f,0.5f };
	miniMapPlayer.size = { 4,4 };
	miniMapPlayer.generateSprite("playerHPGauge.png");

	//ミサイルの残り弾数
	for (int i = 0; i < 8; i++)
	{
		remainingMissileNum[i].anchorpoint = { 0.5f,0.5f };
		remainingMissileNum[i].size = { 200,200 };
		remainingMissileNum[i].rotation = (float)i * 45;
		remainingMissileNum[i].generateSprite("homingGauge.png");
	}

	remainingMissileNum[8].generateSprite("remainingMissileNum.png");

	playerModel.reset(FbxLoader::GetInstance()->LoadmodelFromFile("player"));

	playerObject = new object3dFBX;
	playerObject->initialize();
	playerObject->SetModel(playerModel.get());
	playerObject->SetPosition({ 0,5,0 });
	playerObject->SetScale({ 1,1,1 });
	playerObject->SetScale({ 0.02f,0.02f,0.02f });
	playerObject->setSpeed(2.0f);
	playerObject->setColor({ 1,1,1,1 });

	followCamera = new FollowCamera();

	followCamera->setFollowTarget(playerObject->getPosition(), playerObject->getRotation(), -30);

	followCamera->SetEye({ 0,5,-10 });
	followCamera->SetTarget({ 0,5,0 });

	followCamera->setTargets(playerObject->getPosition(), playerObject->getRotation());

	object3dFBX::SetCamera(followCamera);

	bullet::staticInit();
	Missile::staticInit();

	playerCollision.radius = 2.0f;

	playerHP = maxHP;
	isAlive = true;
}

//移動*
void Player::move()
{
	if (isClearStaging || isOverStaging)
	{
		return;
	}

	if (!isAlive)
	{
		return;
	}

	if (isStop)
	{
		return;
	}

	moveParticlesCount++;

	if (moveParticlesCount % 10 == 0)
	{
		SingleParticle newparticle;
		newparticle.generate();
		newparticle.set(10, playerObject->getPosition(), { 0,0,0 }, { 0,0,0 }, 2.0, 0.0);
		newparticle.color = { 0.5f,0.5f,0.5f,0.2f };
		newparticle.isAddBlend = true;

		particleManagerOnTime::addParticle(newparticle, "effect1.png");
	}

	//ブースト(仮)
	boostMove();

	//オブジェクトの更新
	playerObject->setRotMatrix(XMMatrixIdentity());
	playerCollision.center =
	{
		playerObject->getPosition().x,
		playerObject->getPosition().y,
		playerObject->getPosition().z,
		1.0f
	};

	//自動で前に進み続ける
	playerObject->addMoveFront(followCamera->getFrontVec());

	//マウスでのカメラ操作
	cameraMove();

	//「プレイヤーから見て」右・上・前方向のベクトル
	XMFLOAT3 vSideAxis = getAxis(quaternion(unitX, qLocal));
	XMFLOAT3 vUpAxis = getAxis(quaternion(unitY, qLocal));
	XMFLOAT3 vForwordAxis = getAxis(quaternion(unitZ, qLocal));

	//ロール・ピッチ・ヨーの回転角度を求める
	Quaternion qRoll = quaternion(vForwordAxis, roll);
	Quaternion qPitch = quaternion(vSideAxis, pitch);
	Quaternion qYow = quaternion(vUpAxis, yow);

	//順番にかけていく
	qLocal = qRoll * qLocal;
	qLocal = qPitch * qLocal;
	qLocal = qYow * qLocal;

	//追従
	followCamera->Following(vUpAxis, vForwordAxis, playerObject->getPosition());

	if (!isBoost)
	{
		roll = 0.0f;
	}

	//XMMATRIXに変換したクォータニオンをプレイヤーの回転行列にセット
	playerObject->setRotMatrix(rotate(qLocal));

	playerObject->updata();

	//前への移動量を計算
	followCamera->setFrontVec(moveSpeed);
}

void Player::outArea()
{
	//プレイヤーの座標
	XMFLOAT3 ppos = playerObject->getPosition();

	//原点からの距離を計算
	lengthForPlayerPosition = sqrtf(powf(ppos.x, 2) + powf(ppos.y, 2) + powf(ppos.z, 2));

	//天球の外or地面に近いなら警告
	if (lengthForPlayerPosition >= 400 || ppos.y <= groundPosition.y + 30)
	{
		isOutArea = true;
	}
	else
	{
		//エリア内に戻ったらリセット
		isOutArea = false;
		isCautionDraw = false;
		outAreaCount = 0;
	}

	//エリア外にいる時間をカウント
	if (isOutArea)
	{
		outAreaCount++;

		if (outAreaCount % 10 == 0)
		{
			isCautionDraw = !isCautionDraw;
		}
	}

	//警告を無視し続ける場合は死亡
	if (outAreaCount >= 700)
	{
		playerHP = 0;
	}

	//警告スプライトの更新
	outAreaCaution.spriteUpdata();
}

void Player::boostMove()
{
	if (!isBoostTutorial)
	{
		return;
	}

	if (boostGauge < maxBoostGauge)
	{
		boostGauge += 0.5f;
	}
	else
	{
		boostGauge = maxBoostGauge;
	}

	if (moveSpeed > defaultMoveSpeed)
	{
		moveSpeed -= 0.08f;
	}
	else if (isBoost)
	{
		moveSpeed = defaultMoveSpeed;
		isBoost = false;
	}

	if (boostGauge < 100)
	{
		return;
	}

	if (input->Triger(DIK_SPACE) && !isBoost && moveSpeed == defaultMoveSpeed)
	{
		moveSpeed = boostMoveSpeed;
		boostGauge -= 100;
		yow = 0.0f;
		pitch = 0.0f;
		boostCount++;
		isBoost = true;
	}
}

//カメラ移動
void Player::cameraMove()
{
	if (isClearStaging || isOverStaging)
	{
		return;
	}

	if (isBoost)
	{
		return;
	}

	//ヨー回転
	//右
	if (targetFirst.position.x >= 1000)
	{
		cameraMoveCount++;
		if (yowRotateSpeedPositive < limitRotateSpeed)
		{
			yowRotateSpeedPositive += addRotateSpeed;
		}
	}
	else
	{
		if (yowRotateSpeedPositive > 0)
		{
			yowRotateSpeedPositive += subRotateSpeed;
		}
	}
	//左
	if (targetFirst.position.x <= 280)
	{
		cameraMoveCount++;
		if (yowRotateSpeedNegative > -limitRotateSpeed)
		{
			yowRotateSpeedNegative -= addRotateSpeed;
		}
	}
	else
	{
		if (yowRotateSpeedNegative < 0)
		{
			yowRotateSpeedNegative -= subRotateSpeed;
		}
	}

	//影響が大きい方を適用(いずれ0になるので減速していく)
	if (fabsf(yowRotateSpeedPositive) > fabsf(yowRotateSpeedNegative))
	{
		yow = yowRotateSpeedPositive;
	}
	else
	{
		yow = yowRotateSpeedNegative;
	}

	//ピッチ回転
	//下
	if (targetFirst.position.y >= 620)
	{
		cameraMoveCount++;
		if (pitchRotateSpeedPositive > -limitRotateSpeed)
		{
			pitchRotateSpeedPositive -= addRotateSpeed;
		}
	}
	else
	{
		if (pitchRotateSpeedPositive < 0)
		{
			pitchRotateSpeedPositive -= subRotateSpeed;
		}
	}
	//上
	if (targetFirst.position.y <= 100)
	{
		cameraMoveCount++;
		if (pitchRotateSpeedNegative < limitRotateSpeed)
		{
			pitchRotateSpeedNegative += addRotateSpeed;
		}
	}
	else
	{
		if (pitchRotateSpeedNegative > 0)
		{
			pitchRotateSpeedNegative += subRotateSpeed;
		}
	}

	//影響が大きい方を適用(いずれ0になるので減速していく)
	if (fabsf(pitchRotateSpeedPositive) > fabsf(pitchRotateSpeedNegative))
	{
		pitch = pitchRotateSpeedPositive;
	}
	else
	{
		pitch = pitchRotateSpeedNegative;
	}
}

void Player::playerClearMove()
{
	if (!isClearStaging)
	{
		return;
	}

	if (!isStagingSet)
	{
		return;
	}

	clearTime++;

	if (clearTime == maxClearTime)
	{
		isClearStaging = false;
	}
}

void Player::playerDeathMove()
{
	if (!isOverStaging)
	{
		return;
	}

	if (!isStagingSet)
	{
		return;
	}

	fallCount++;

	if (fallCount == maxFallCount)
	{
		isOverStaging = false;
		return;
	}

#pragma region 黒煙パーティクル
	if (fallCount % 30 == 0)
	{
		SingleParticle newparticle;
		newparticle.generate();

		XMFLOAT3 smokePos = playerObject->getPosition();

		smokePos =
		{
			smokePos.x + (float)(rand() % 6 - 3),
			smokePos.y + (float)(rand() % 6 - 3),
			smokePos.z + (float)(rand() % 6 - 3)
		};
		newparticle.set(100, smokePos, { 0,0,0 }, { 0,0,0 }, 5.0, 2.0);

		particleManagerOnTime::addParticle(newparticle, "smoke.png");
	}
#pragma endregion 黒煙パーティクル

	//回転しながら落ちて行く
	fallRot.x = 0.05;
	fallRot.y = 0.05;
	fallRot.z = 0.05;
	playerObject->setRotMatrix(fallRot.x, fallRot.y, fallRot.z);

	//playerObject->addMoveFront({ 0,-0.03,0 });

	fallScale.x -= 0.0000555f;
	fallScale.y -= 0.0000555f;
	fallScale.z -= 0.0000555f;
	playerObject->SetScale(fallScale);

	playerObject->updata();

	//カメラ処理
	followCamera->SetTarget(playerObject->getPosition());

	followCamera->Update();
}

void Player::setStaging(bool isclear)
{
	if (isStagingSet)
	{
		return;
	}

	XMFLOAT3 pos = playerObject->getPosition();

	followCamera->SetEye({ pos.x,pos.y + 20,pos.z - 10 });

	fallScale = playerObject->getScale();

	if (isclear)
	{
		isClearStaging = true;
		isOverStaging = false;
	}
	else
	{
		isClearStaging = false;
		isOverStaging = true;

		fallCount = 0;
	}

	isStagingSet = true;
}

//更新
void Player::updata()
{
	//死んだ弾は削除
	bulletsList.remove_if([](std::unique_ptr<bullet>& newbullet)
		{
			return newbullet->isAlive == false;
		});

	missilesList.remove_if([](std::unique_ptr<Missile>& newmissile)
		{
			return newmissile->isAlive == false;
		});

	missilesList.remove_if([](std::unique_ptr<Missile>& newmissile)
		{
			return newmissile->enemyPointer == nullptr;
		});

	//移動
	move();

	//エリア外判定
	outArea();

	//ターゲットカーソルの処理
	targetUpdata();

	//クリア＆ゲームオーバー演出
	playerClearMove();
	playerDeathMove();

	if (isClearStaging || isOverStaging)
	{
		return;
	}

	//攻撃を連続したフレームで食らわないようにする
	if (isArmor)
	{
		armorTime++;

		if (armorTime % 10 == 0)
		{
			isInvisible *= -1;
		}

		if (armorTime >= maxArmorTime)
		{
			armorTime = 0;
			isArmor = false;
			isInvisible = -1;
		}
	}
	else
	{
		armorTime = 0;
	}

	//ダメージ表現スプライト
	damage.spriteUpdata();

	//HPゲージの更新
	HPGaugeBar.size = { (float)playerHP * 40,20 };
	HPGaugeBar.spriteUpdata();

	boostGaugeBar.size = { (float)boostGauge / 1.5f,20 };
	boostGaugeBar.spriteUpdata();

	dangarHPGaugeBar.spriteUpdata();

	gaugeFrame.spriteUpdata();

	//HPが4未満ならHPバー点滅
	if (playerHP < 4)
	{
		cautionHPCount++;

		if (cautionHPCount % 7 == 0)
		{
			isDangerHP = !isDangerHP;
		}
	}
	else
	{
		cautionHPCount = 0;
		isDangerHP = false;
	}

	//ミニマップアイコン更新
	XMFLOAT3 playerPosition = playerObject->getPosition();

	//上から見た位置なので X・Z座標
	XMFLOAT3 minimapPosition =
	{
		(playerPosition.x * 0.21f) + Enemy::miniMapPosition.x,
		(playerPosition.z * 0.21f) + Enemy::miniMapPosition.x,
		0
	};

	miniMapPlayer.position = minimapPosition;
	miniMapPlayer.spriteUpdata();

	if (playerHP <= 0)
	{
		isAlive = false;
		setStaging(false);
	}

	//通常弾の更新(ユニークリスト)
	for (std::unique_ptr<bullet>& bullet : bulletsList)
	{
		bullet->update();
	}

	//ミサイルの更新(ユニークリスト)
	for (std::unique_ptr<Missile>& missile : missilesList)
	{
		missile->update();
	}
}

void Player::targetUpdata()
{
	if (isStop)
	{
		return;
	}

	if (!isAlive)
	{
		return;
	}

	if (isClearStaging || isOverStaging)
	{
		return;
	}

	//マウスの移動量をターゲットカーソルの座標に加算
	targetFirst.position.x += input->mouseMoveVecrocity.x;
	targetFirst.position.y += input->mouseMoveVecrocity.y;

	//リリース時のみマウスカーソル固定
#ifdef _DEBUG
	SetCursorPos(mouseOffsetX, mouseOffsetY);
#else
	SetCursorPos(mouseOffsetX, mouseOffsetY);
#endif // DEBUG

	//ターゲットカーソルが場外にいかないように制御
	if (targetFirst.position.x <= 29)
	{
		targetFirst.position.x = 29;
	}
	if (targetFirst.position.x >= 1251)
	{
		targetFirst.position.x = 1251;
	}
	if (targetFirst.position.y <= 29)
	{
		targetFirst.position.y = 29;
	}
	if (targetFirst.position.y >= 691)
	{
		targetFirst.position.y = 691;
	}

	//プレイヤーのスクリーン座標
	XMFLOAT2 PlayerScreenPosition = playerObject->worldToScleen();

	XMFLOAT2 secondTargetPos =
	{
		PlayerScreenPosition.x + (targetFirst.position.x - PlayerScreenPosition.x) * 0.666f,
		PlayerScreenPosition.y + (targetFirst.position.y - PlayerScreenPosition.y) * 0.666f,
	};

	targetSecond.position =
	{
		secondTargetPos.x,
		secondTargetPos.y,
		0.0f
	};

	XMFLOAT2 thirdTargetPos =
	{
		PlayerScreenPosition.x + (targetFirst.position.x - PlayerScreenPosition.x) * 0.333f,
		PlayerScreenPosition.y + (targetFirst.position.y - PlayerScreenPosition.y) * 0.333f,
	};

	targetThird.position =
	{
		thirdTargetPos.x,
		thirdTargetPos.y,
		0.0f
	};

	targetFirst.spriteUpdata();
	targetSecond.spriteUpdata();
	targetThird.spriteUpdata();

	//左クリックで通常弾
	if (input->Mouse_LeftTriger() && isNormalShot)
	{
		//リスト化
		std::unique_ptr<bullet> newBullet = std::make_unique<bullet>();
		newBullet->init();
		newBullet->set(playerObject->getPosition(),
			playerObject->screenToWorld({ targetFirst.position.x,targetFirst.position.y }));

		bulletsList.push_back(std::move(newBullet));
		normalShotCount++;
	}

	//ロックオンモードに切り替え
	if (input->Mouse_LeftPush() && isHomingMissile)
	{
		targetCount++;
	}
	else
	{
		targetCount = 0;
	}

	rockonGauge[0].size.x = (targetCount - 5) * 3.5f;
	rockonGauge[1].size.x = (targetCount - 5) * 3.5f;

	rockonGauge[0].spriteUpdata();
	rockonGauge[1].spriteUpdata();

	if (targetCount > 70)
	{
		targetFirst.rotation -= 7.0f;
		targetSecond.rotation += 7.0f;
		targetThird.rotation -= 7.0f;
		isRockOn = true;
	}
	else
	{
		targetFirst.rotation += 4.0f;
		targetSecond.rotation -= 4.0f;
		targetThird.rotation += 4.0f;
		isRockOn = false;
	}

	//ミサイル残弾数の表示
	for (int i = 0; i < 8; i++)
	{
		remainingMissileNum[i].position = { 640,360,0 };
		remainingMissileNum[i].spriteUpdata();
	}

	remainingMissileNum[8].anchorpoint = { 0,0 };
	remainingMissileNum[8].position = { 640 + (remainingMissileNum[8].size.x / 2),360,0 };
	remainingMissileNum[8].texLeftTop = { 0,0 };
	remainingMissileNum[8].texSize = { 100,100 };
	remainingMissileNum[8].size = { 70,70 };

	remainingMissileNum[8].spriteUpdata(true);
}

void Player::addMissile(Enemy* enemy, int& targetnum)
{
	if (isClearStaging || isOverStaging)
	{
		return;
	}

	if (targetnum <= 0)
	{
		isShotMissile = false;
		waitMissileTime = nextMissileTime;
		return;
	}

	if (!enemy->isTargetSet || enemy->isSetMissile)
	{
		return;
	}

	waitMissileTime--;

	if (waitMissileTime <= 0)
	{
		//ミサイル追加
		std::unique_ptr<Missile> newMissile = std::make_unique<Missile>();
		newMissile->init();
		newMissile->setPenemy(enemy);
		newMissile->start(playerObject->getPosition());
		missilesList.push_back(std::move(newMissile));

		//発射パーティクル追加
		SingleParticle newparticle;
		newparticle.generate();
		newparticle.set(5, playerObject->getPosition(), { 0,0,0 }, { 0,0,0 }, 6.0, 6.0);
		newparticle.color = { 1,1,1,0.5f };
		newparticle.isAddBlend = true;
		particleManagerOnTime::addParticle(newparticle, "effect1.png");

		enemy->isSetMissile = true;

		targetnum--;

		waitMissileTime = nextMissileTime;

		//ミサイルを撃った数をカウント(チュートリアル)
		missileCount++;
	}
}

//リセット
void Player::reset()
{
	isAlive = true;
	isStagingSet = false;
	playerHP = maxHP;
	isBoost = false;
	boostGauge = maxBoostGauge;
	moveSpeed = defaultMoveSpeed;
	armorTime = 0;
	isArmor = false;
	isInvisible = -1;
	isDangerHP = false;
	cautionHPCount = 0;
	playerObject->SetPosition({ 0,5,0 });
	playerObject->SetScale({ 0.02f,0.02f,0.02f });
	playerObject->setRotMatrix(XMMatrixIdentity());
	roll = 0.0f;
	pitch = 0.0f;
	yow = 0.0f;
	qLocal = quaternion();
	followCamera = new FollowCamera();
	followCamera->setFollowTarget(playerObject->getPosition(), playerObject->getRotation(), -30);
	followCamera->SetEye({ 0,5,-10 });
	followCamera->SetTarget({ 0,5,0 });
	followCamera->setTargets(playerObject->getPosition(), playerObject->getRotation());

	//「プレイヤーから見て」右・上・前方向のベクトル
	XMFLOAT3 vSideAxis = getAxis(quaternion(unitX, qLocal));
	XMFLOAT3 vUpAxis = getAxis(quaternion(unitY, qLocal));
	XMFLOAT3 vForwordAxis = getAxis(quaternion(unitZ, qLocal));

	//ロール・ピッチ・ヨーの回転角度を求める
	Quaternion qRoll = quaternion(vForwordAxis, roll);
	Quaternion qPitch = quaternion(vSideAxis, pitch);
	Quaternion qYow = quaternion(vUpAxis, yow);

	//順番にかけていく
	qLocal = qRoll * qLocal;
	qLocal = qPitch * qLocal;
	qLocal = qYow * qLocal;

	//追従
	followCamera->Following(vUpAxis, vForwordAxis, playerObject->getPosition());

	targetFirst.position = { (float)mouseOffsetX,(float)mouseOffsetY ,0 };
	SetCursorPos(mouseOffsetX, mouseOffsetY);
	cameraMoveCount = 0;
	object3dFBX::SetCamera(followCamera);
}

//描画
void Player::draw3D(directX* directx)
{
	//プレイヤー本体の描画
	if (isInvisible == -1)
	{
		playerObject->Draw(directx->cmdList.Get());
	}

	if (isStop)
	{
		return;
	}

	if (!isAlive)
	{
		return;
	}

	//通常弾の描画(ユニークリスト)
	for (std::unique_ptr<bullet>& bullet : bulletsList)
	{
		bullet->draw(directx);
	}
	//ミサイルの描画(ユニークリスト)
	for (std::unique_ptr<Missile>& missile : missilesList)
	{
		missile->draw(directx);
	}
}

void Player::draw2D(directX* directx, int targetnum)
{
	if (!isAlive)
	{
		return;
	}

	if (isStop)
	{
		return;
	}

	targetThird.drawSprite(directx->cmdList.Get());
	targetSecond.drawSprite(directx->cmdList.Get());
	targetFirst.drawSprite(directx->cmdList.Get());

	if (isArmor && armorTime < 5)
	{
		damage.drawSprite(directx->cmdList.Get());
	}

	if (targetCount > 5 && targetCount <= 70)
	{
		rockonGauge[0].drawSprite(directx->cmdList.Get());
		rockonGauge[1].drawSprite(directx->cmdList.Get());
	}

	if (isRockOn)
	{
		for (int i = 0; i < MaxPlayerMissileNum - targetnum; i++)
		{
			remainingMissileNum[i].drawSprite(directx->cmdList.Get());
		}

		if (targetnum >= MaxPlayerMissileNum)
		{
			remainingMissileNum[8].drawSprite(directx->cmdList.Get());
		}
	}

	gaugeFrame.drawSprite(directx->cmdList.Get());
	boostGaugeBar.drawSprite(directx->cmdList.Get());
	if (isDangerHP)
	{
		dangarHPGaugeBar.drawSprite(directx->cmdList.Get());
	}
	HPGaugeBar.drawSprite(directx->cmdList.Get());

	if (isCautionDraw)
	{
		outAreaCaution.drawSprite(directx->cmdList.Get());
	}
}

void Player::drawMiniMapIcon(directX* directx)
{
	if (!isAlive)
	{
		return;
	}

	if (isStop)
	{
		return;
	}

	miniMapPlayer.drawSprite(directx->cmdList.Get());
}
