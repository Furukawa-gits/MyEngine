#pragma once
#include<DirectXMath.h>

using namespace DirectX;

class RKDVector3 :public XMFLOAT3
{
public:
	//XMFLOAT3構造体で定義
	//float x;	//x成分
	//float y;	//y成分
	//float z;	//z成分

public:
	//コンストラクタ
	RKDVector3();								//零ベクトル
	RKDVector3(float x, float y, float z);		//各成分を指定した生成

	//メンバ関数
	float length()const;					//長さ
	RKDVector3& nomalize();					//正規化
	float dot(const RKDVector3& v)const;		//内積
	RKDVector3 cross(const RKDVector3& v)const;	//外積


	//単項演算子オーバーロード
	RKDVector3 operator+()const;
	RKDVector3 operator-()const;

	//代入演算子オーバーロード
	RKDVector3& operator=(const XMFLOAT3& v);
	RKDVector3& operator+=(const RKDVector3& v);
	RKDVector3& operator-=(const RKDVector3& v);
	RKDVector3& operator*=(float s);
	RKDVector3& operator/=(float s);
};

//二項演算子オーバーロード
//各パターンを準備
const RKDVector3 operator+(const RKDVector3& v1, const RKDVector3& v2);
const RKDVector3 operator-(const RKDVector3& v1, const RKDVector3& v2);
const RKDVector3 operator*(const RKDVector3& v, float s);
const RKDVector3 operator*(float s, const RKDVector3& v);
const RKDVector3 operator/(const RKDVector3& v, float s);