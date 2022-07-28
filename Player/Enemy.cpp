#include"Enemy.h"
#include<random>

#pragma region �G�̒e
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

	testCube = FbxLoader::GetInstance()->LoadmodelFromFile("testEnemy_01");

	testObject = new Object3d_FBX;
	testObject->Initialize();
	testObject->SetModel(testCube);
	testObject->SetScale({ 1.0f,1.0f,1.0f });
	testObject->setColor({ (float)(rand() % 100) / 100,(float)(rand() % 100) / 100 ,(float)(rand() % 100) / 100 ,1 });
	//testObject->PlayAnimation();
}

void Enemy::set(XMFLOAT3 pos)
{
	position = pos;
	testObject->SetPosition(pos);
	Isarive = true;
	Istarget_set = false;
}

void Enemy::reSet()
{
	testObject->SetPosition(position);
	Isarive = true;
	HP = 1;
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

	//HP��0�ɂȂ��������
	if (HP <= 0)
	{
		Isarive = false;
	}

	if (Istarget_set)
	{
		Rock_Target.rotation += 1.5f;
	}

	Rock_Target.SpriteTransferVertexBuffer();
	Rock_Target.SpriteUpdate();

	testObject->Update();
}

void Enemy::chase(XMFLOAT3 pPos)
{
	XMFLOAT3 dis =
	{
		pPos.x - position.x,
		pPos.y - position.y,
		pPos.z - position.z
	};

	float disLength = sqrtf(powf(dis.x, 2) + powf(dis.y, 2) + powf(dis.z, 2));

	XMFLOAT3 disNormal =
	{
		dis.x / disLength,
		dis.y / disLength,
		dis.z / disLength
	};


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
		HP--;

		if (HP <= 0)
		{
			Isarive = false;
		}
	}
}
