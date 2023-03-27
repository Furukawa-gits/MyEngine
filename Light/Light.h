#pragma once
//#include"3d/Mesh.h"
#include <wrl.h>
#include <DirectXMath.h>
#include <d3dx12.h>

class Light
{
private://�G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	//�萔�o�b�t�@�\����
	struct ConstBufferData
	{
		XMVECTOR lightv;		//���C�g�̕���
		XMFLOAT3 lightcolor;	//���C�g�̐F
	};

private://�ÓI�����o�ϐ�
	static ID3D12Device* device;

public://�ÓI�����o�֐�

	/// <summary>
	/// �ÓI������
	/// </summary>
	/// <param name="device">�f�o�C�X</param>
	static void Staticinitialize(ID3D12Device* device);

	/// <summary>
	/// �萔�o�b�t�@�]��
	/// </summary>
	void TransferConstBuffer();

	/// <summary>
	/// ������
	/// </summary>
	void init();

	/// <summary>
	/// ���C�g�������Z�b�g
	/// </summary>
	/// <param name="lightdir">���C�g����</param>
	void SetLightDir(const XMVECTOR& lightdir);

	/// <summary>
	/// ���C�g�F���Z�b�g
	/// </summary>
	/// <param name="lightcolor">���C�g�F</param>
	void SetLightColor(const XMFLOAT3& lightcolor);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(ID3D12GraphicsCommandList* cmdlist, UINT rootParameterindex);

	/// <summary>
	/// �C���X�^���X����
	/// </summary>
	/// <returns>�C���X�^���X</returns>
	static Light* Create();



private://�����o�ϐ�
	//�萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuff;
	//���C�g��������
	XMVECTOR lightdir = { 1,0,0,0 };
	//���C�g�F
	XMFLOAT3 lightcolor = { 1,1,1 };
	//�_�[�e�B�t���O
	bool dirty = false;

};