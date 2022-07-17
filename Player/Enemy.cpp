#include"Enemy.h"

#pragma region “G‚Ì’e
void enemy_bullet::init(int index)
{

}

void enemy_bullet::set(XMFLOAT3 start_pos, XMFLOAT3 Target)
{

}

void enemy_bullet::update()
{

}

void enemy_bullet::draw()
{

}
#pragma endregion

void Enemy::init(int enemy_index, int enemy_bullet_index)
{
	Rock_Target.anchorpoint = { 0.5f,0.5f };
	Rock_Target.size = { 70,70 };
	Rock_Target.GenerateSprite("Rock_on.png");

	testCube = FbxLoader::GetInstance()->LoadmodelFromFile("boneTest");

	testObject = new Object3d_FBX;
	testObject->Initialize();
	testObject->SetModel(testCube);
	testObject->SetScale({ 0.5f,0.5f,0.5f });
	//testObject->PlayAnimation();
}

void Enemy::set(XMFLOAT3 pos)
{
	startPos = pos;
	testObject->SetPosition(pos);
	Isarive = true;
	Istarget_set = false;
}

void Enemy::reSet()
{
	testObject->SetPosition(startPos);
	Isarive = true;
	Istarget_set = false;
}

void Enemy::draw3D(directX* directx)
{
	if (!Isarive)
	{
		return;
	}
	testObject->Draw(directx->cmdList.Get());
}

void Enemy::drawSp(directX* directx)
{
	if (Isarive && Istarget_set)
	{
		Rock_Target.DrawSprite(directx->cmdList.Get());
	}
}

void Enemy::update(XMFLOAT3 Player_pos)
{
	if (!Isarive)
	{
		return;
	}

	if (Istarget_set)
	{
		Rock_Target.rotation += 1.5f;
	}

	Rock_Target.SpriteTransferVertexBuffer();
	Rock_Target.SpriteUpdate();

	testObject->Update();
}

void Enemy::isHitTarget(XMFLOAT2 targetpos, bool istarget)
{
	if (!Isarive)
	{
		return;
	}

	XMFLOAT2 screenPos = testObject->worldToScleen();

	float dis = sqrtf(powf(targetpos.x - screenPos.x, 2) + powf(targetpos.y - screenPos.y, 2));

	if (dis < 20 && istarget && !Istarget_set)
	{
		Istarget_set = true;
	}

	Rock_Target.position = { screenPos.x,screenPos.y,0 };
}

void Enemy::isHitShot(XMFLOAT2 targetpos)
{
	if (!Isarive)
	{
		return;
	}

	XMFLOAT2 screenPos = testObject->worldToScleen();

	float dis = sqrtf(powf(targetpos.x - screenPos.x, 2) + powf(targetpos.y - screenPos.y, 2));

	if (dis < 30)
	{
		Isarive = false;
	}
}
