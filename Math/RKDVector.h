#pragma once
#include<DirectXMath.h>

using namespace DirectX;

class RKDVector3 :public XMFLOAT3
{
public:
	//XMFLOAT3�\���̂Œ�`
	//float x;	//x����
	//float y;	//y����
	//float z;	//z����

public:
	//�R���X�g���N�^
	RKDVector3();								//��x�N�g��
	RKDVector3(float x, float y, float z);		//�e�������w�肵������

	//�����o�֐�
	float length()const;					//����
	RKDVector3& nomalize();					//���K��
	float dot(const RKDVector3& v)const;		//����
	RKDVector3 cross(const RKDVector3& v)const;	//�O��


	//�P�����Z�q�I�[�o�[���[�h
	RKDVector3 operator+()const;
	RKDVector3 operator-()const;

	//������Z�q�I�[�o�[���[�h
	RKDVector3& operator+=(const RKDVector3& v);
	RKDVector3& operator-=(const RKDVector3& v);
	RKDVector3& operator*=(float s);
	RKDVector3& operator/=(float s);
};

//�񍀉��Z�q�I�[�o�[���[�h
//�e�p�^�[��������
const RKDVector3 operator+(const RKDVector3& v1, const RKDVector3& v2);
const RKDVector3 operator-(const RKDVector3& v1, const RKDVector3& v2);
const RKDVector3 operator*(const RKDVector3& v, float s);
const RKDVector3 operator*(float s, const RKDVector3& v);
const RKDVector3 operator/(const RKDVector3& v, float s);