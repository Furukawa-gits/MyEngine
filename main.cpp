#include"Scene/GameScene.h"
#include"Base/Fps_Manager.h"


// Windows�A�v���ł̃G���g���[�|�C���g(main�֐�)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	Window_Generate* Win = new Window_Generate;

	Win->Win_Init();

	Win->Win_Create();

	// DirectX�����������@��������

//�f�o�b�O���C���[���I����
#ifdef _DEBUG
	ComPtr<ID3D12Debug> debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
	{
		debugController->EnableDebugLayer();
	}
#endif

	//directX�N���X
	directX directx;

	directx.Init(Win);
	directx.Initializedepth();

	//��������
	Audio audio;
	audio.Initialize();

	//���̓N���X
	dxinput input;

	//������
	input.init(Win->w, Win->hwnd);

	//3d�I�u�W�F�N�g�ÓI������
	object3D::StaticInit();

	//FPS����
	FpsManager fps;

	//�Q�[���V�[��������
	GameScene gamescene;
	gamescene.Init(&directx, &input, &audio);

	//FPS����(����)
	fps.init();

	/*-----------------------------------------------�@�Q�@�[�@���@���@�[�@�v�@--------------------------------------------------------*/
	while (true)
	{
		// ���b�Z�[�W������H
		if (PeekMessage(&Win->msg, nullptr, 0, 0, PM_REMOVE)) {
			TranslateMessage(&Win->msg); // �L�[���̓��b�Z�[�W�̏���
			DispatchMessage(&Win->msg); // �v���V�[�W���Ƀ��b�Z�[�W�𑗂�
		}

		// �~�{�^���ŏI�����b�Z�[�W��������Q�[�����[�v�𔲂���
		if (Win->msg.message == WM_QUIT) {
			break;
		}

		fps.Run();

		// DirectX���t���[�������@��������

		//�L�[�{�[�h���̎擾
		input.Update(Win->hwnd);

		//�Q�[���V�[���X�V
		gamescene.Update();

		//ESC�L�[�Ŕ�����
		if (input.Triger(DIK_ESCAPE) || gamescene.Isclose == true)
		{
			break;
		}

		// �`�揈���O
		directx.Begin_Draw();

		//�Q�[���V�[���`��
		gamescene.Draw();

		// �`�揈����
		directx.Finish_Draw();
	}

	//audio.UnLoadSoundWave(&gamescene.Title_bgm);

	// �E�B���h�E�N���X��o�^����
	Win->Win_Deleate();

	return 0;
}
