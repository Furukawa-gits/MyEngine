#include"Enemy.h"
#include<random>

#pragma region GÌe
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
	Isarive = false;

	Rock_Target.anchorpoint = { 0.5f,0.5f };
	Rock_Target.size = { 70,70 };
	Rock_Target.GenerateSprite("Rock_on.png");

	testCube = FbxLoader::GetInstance()->LoadmodelFromFile("testEnemy_01");

	testObject = new Object3d_FBX;
	testObject->Initialize();
	testObject->SetModel(testCube);
	testObject->SetScale({ 1.0f,1.0f,1.0f });

	enemy_collision.radius = 2.0f;
}

void Enemy::set(XMFLOAT3 pos)
{
	position = pos;
	startPosition = pos;
	testObject->SetPosition(pos);
	Isarive = true;
	Istarget_set = false;
}

void Enemy::reSet()
{
	testObject->SetPosition(startPosition);
	position = startPosition;
	Isarive = true;
	HP = 1;
	Istarget_set = false;
	IsSetMissile = false;
}

void Enemy::draw3D(directX* directx)
{
	if (!Isarive)
	{
		return;
	}
	testObject->SetPipelineSimple(directx->cmdList.Get());
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

	//ÇöJEgÁZ
	homingCount++;

	if (homingCount % 30 == 0)
	{
		enemy_speed = 0.1f;
	}

	if (homingCount % 33 == 0)
	{
		enemy_speed = 0.05f;
	}

	position = testObject->getPosition();
	XMFLOAT3 dis = { Player_pos.x - position.x,Player_pos.y - position.y,Player_pos.z - position.z };

	float lengthDis = sqrtf(powf(dis.x, 2) + powf(dis.y, 2) + powf(dis.z, 2));

	dis.x /= lengthDis;
	dis.y /= lengthDis;
	dis.z /= lengthDis;

	position.x += dis.x * enemy_speed;
	position.y += dis.y * enemy_speed;
	position.z += dis.z * enemy_speed;

	//HPª0ÉÈÁ½çÁÅ
	if (HP <= 0)
	{
		Isarive = false;
	}

	if (Istarget_set)
	{
		Rock_Target.rotation += 1.5f;
		XMFLOAT2 screenPos = testObject->worldToScleen();
		Rock_Target.position = { screenPos.x,screenPos.y,0 };
	}

	Rock_Target.SpriteTransferVertexBuffer();
	Rock_Target.SpriteUpdate();

	//testObject->SetPosition(position);
	enemy_collision.center = 
	{ 
		testObject->getPosition().x,
		testObject->getPosition().y,
		testObject->getPosition().z,1.0f 
	};
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
