#pragma once
#include"../Base/WindowGenerate.h"
#include"../Base/DirectX_Base.h"
#include"../camera/Camera.h"
#include"../staging/Easing.h"
#include<DirectXTex.h>
#include <d3dcompiler.h>
#include<string>
#include<map>

#pragma comment(lib, "d3dcompiler.lib")

using namespace Microsoft::WRL;

class SingleParticle
{
private:
	// ���_�f�[�^�\����
	struct VertexPosNormalUv
	{
		XMFLOAT3 pos; // xyz���W
		XMFLOAT3 normal; // �@���x�N�g��
		XMFLOAT2 uv;  // uv���W
	};

	//���_�f�[�^�\����(���W�̂�)
	struct VertexPos
	{
		XMFLOAT3 pos;//xyz���W
		float scale;
	};

	// �萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferData
	{
		XMMATRIX mat;	// �R�c�ϊ��s��
		XMMATRIX matBillboard;
	};

public:
	static void particleStaticInit(directX* Directx, Camera* Camera);

	static void setCamera(Camera* setcamera);

	static void createPipeline();

	static void createPipelineAddBlend();

	static void loadTexInMap(const std::string& filepath);

	void generate();

	void loadTexture(const std::string& filepath);

	/// <summary>
	/// �Z�b�e�B���O
	/// </summary>
	/// <param name="life">����</param>
	/// <param name="position">�����ʒu</param>
	/// <param name="velocity">�ړ��������</param>
	/// <param name="accel">�����x</param>
	/// <param name="start_scale">�����T�C�Y</param>
	/// <param name="end_scale">�ŏI�T�C�Y</param>
	/// <param name="isextinctionframe">�����ŏ����邩�ǂ���</param>
	/// <param name="issetpos">�C���X�^���X���ō��W���Z�b�g���邩�ǂ���</param>
	void set(int life, XMFLOAT3 position, XMFLOAT3 velocity, XMFLOAT3 accel, float start_scale, float end_scale, bool isextinctionframe = true, bool issetpos = false);

	static void UpdateViewMatrix();

	void updata();

	void setPosition(XMFLOAT3 setposition)
	{
		if (!isSetPosition)
		{
			return;
		}

		position = setposition;
	}

	void draw();

	void drawSpecifyTex(const std::string texturename);

	void setPiplineAddBlend();

	void resetPipeline();

	void setIsActive(bool isactive) { isActive = isactive; }

	bool getIsActive() { return isActive; }

public:
	//���W
	XMFLOAT3 position = {};
	//���x
	XMFLOAT3 velocity = {};
	//�����x
	XMFLOAT3 accel = {};
	//�F
	XMFLOAT4 color = { 1,1,1,1 };
	//�X�P�[��
	float scale = 1.0f;
	//�����l
	float s_scale = 1.0f;
	//�ŏI�l
	float e_scale = 0.0f;
	//���݃t���[��
	int frame = 0;
	//�I���t���[��
	int num_frame = 0;
	//���ł��邩�ǂ���
	bool isActive = false;
	//���ł���������t���[�����ǂ���
	bool isExtinctionFrame = true;
	//�C���X�^���X����position���Z�b�g���邩�ǂ���
	bool isSetPosition = false;
	// �萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuff;
	// �e�N�X�`���o�b�t�@
	ComPtr<ID3D12Resource> texbuff;
	// �f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeap;
	// ���_�o�b�t�@
	ComPtr<ID3D12Resource> vertBuff;
	// ���_�f�[�^�z��
	VertexPos vertices;
	// ���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView;

private:
	// �ÓI�����o�ϐ�
	// �f�o�C�X
	static directX* directx;
	// ���[�g�V�O�l�`��
	static ComPtr<ID3D12RootSignature> rootSignature;
	static ComPtr<ID3D12RootSignature> rootSignatureAddBlend;
	// �p�C�v���C���X�e�[�g�I�u�W�F�N�g
	static ComPtr<ID3D12PipelineState> pipeLineState;
	static ComPtr<ID3D12PipelineState> pipeLineStateAddBlend;
	// �r���[�s��
	static XMMATRIX matView;
	//�r���{�[�h�s��
	static XMMATRIX matBillboard;
	//Y������r���{�[�h�s��
	static XMMATRIX matBillboardY;
	//�J����
	static Camera* camera;

	//�e�N�X�`���i�[�}�b�v
	static std::map<std::string, ComPtr<ID3D12DescriptorHeap>> texDescMap;
	static std::map<std::string, ComPtr<ID3D12Resource>> texBufMap;

	//�ʏ펞�ȊO�̃p�C�v���C�����Z�b�g���Ă��邩
	bool isSetOtherPipeline = false;
};