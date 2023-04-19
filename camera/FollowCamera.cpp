#include"FollowCamera.h"
#include"../Base/WindowGenerate.h"

FollowCamera::FollowCamera()
{
	aspectRatio = (float)windowWidth / windowHight;

	//�r���[�s��̌v�Z
	UpdateViewMatrix();

	// �ˉe�s��̌v�Z
	UpdateProjectionMatrix();

	// �r���[�v���W�F�N�V�����̍���
	matViewProjection = matView * matProjection;
}

void FollowCamera::setFollowTarget(XMFLOAT3 TargetPos,XMFLOAT3 TargetAngle, const float TargetDis)
{
	//�^�[�Q�b�g�Ώۂ��Z�b�g
	TargetObjectPos = &TargetPos;
	TargetObjectAngle = &TargetAngle;
	TargetDistance = TargetDis;
	target = *TargetObjectPos;
	eye = { target.x,target.y,target.z + TargetDistance };
	up = { 0,1,0 };
}

void FollowCamera::Following(XMFLOAT3 upVector, XMFLOAT3 forwordVector, XMFLOAT3 targetPos)
{
	//�Ǐ]����^�[�Q�b�g�̍��W���Z�b�g
	target = targetPos;
	XMVECTOR targetPosition = XMLoadFloat3(&target);

	//�N�H�[�^�j�I����roll���𔽑Ό����ɂ����x�N�g�������
	XMFLOAT3 backVector = { -forwordVector.x,-forwordVector.y ,-forwordVector.z };

	//��̃x�N�g���𐳋K������苗������
	float backLength = sqrtf(powf(backVector.x, 2) + powf(backVector.y, 2) + powf(backVector.z, 2));
	backLength = 1 / backLength;
	backVector = {
		backVector.x * backLength * TargetDistance,
		backVector.y * backLength * TargetDistance,
		backVector.z * backLength * TargetDistance
	};

	//�^�[�Q�b�g�̍��W�����̃x�N�g���������ʒu�ɃJ�������Z�b�g(�O�l�̎��_)
	XMVECTOR eyePosition = targetPosition + XMLoadFloat3(&backVector);

	//�r���[�s�񐶐�
	matView = XMMatrixLookAtLH(eyePosition, targetPosition, XMLoadFloat3(&upVector));

	//�v���W�F�N�V�����s�񐶐�
	UpdateProjectionMatrix();

	//�r���[�s��~�v���W�F�N�V�����s��
	matViewProjection = matView * matProjection;

	//���N���X��target�Eeye�Eup�Ɍv�Z�������ʂ��Z�b�g
	XMStoreFloat3(&target, targetPosition);
	XMStoreFloat3(&eye, eyePosition);
	XMStoreFloat3(&up, XMLoadFloat3(&upVector));

	//���J�����N���X�̍X�V����
	Update();

	//�����Ă�������ɐi�ސ��K���x�N�g���𐶐�
	frontVec = {
		target.x - eye.x,
		target.y - eye.y,
		target.z - eye.z
	};
	float length = sqrtf(powf(frontVec.x, 2) + powf(frontVec.y, 2) + powf(frontVec.z, 2));
	length = 1 / length;
	frontVec = {
		frontVec.x * length,
		frontVec.y * length,
		frontVec.z * length
	};
}