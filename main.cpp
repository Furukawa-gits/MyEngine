#include"Scene/GameScene.h"
#include"2D/PostEffect.h"
#include"Base/Fps_Manager.h"
#include"FbxLoder/FbxLoader.h"
#include"Scene/sceneManager.h"


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
	directx.initializeDepth();

	FbxLoader::GetInstance()->Initilize(directx.dev.Get());

	//��������
	Audio audio;
	audio.Initialize();

	//���̓N���X
	dxinput input;

	//������
	input.init(Win->w, Win->hwnd);

	//3d�I�u�W�F�N�g�ÓI������
	//object3Dobj::staticInit();

	//�Q�[���V�[��������
	//GameScene gamescene;
	//gamescene.init(&directx, &input, &audio);

	//�V�[���}�l�[�W���[������
	primitiveScene::setStaticData(&directx, &input, &audio);
	sceneManager* scene = new sceneManager();

	PostEffect* posteffect = new PostEffect();
	posteffect->Init(directx.dev.Get());

	bool isSetGray = false;

	//FPS����
	FpsManager fps;

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
		//gamescene.updata();

		scene->nowscene->updata();
		scene->replacementScene();

		//�O���[�X�P�[���؂�ւ�
		if (input.Triger(DIK_2) && isSetGray == false)
		{
			//isSetGray = true;
		}
		else if (input.Triger(DIK_2) && isSetGray == true)
		{
			//isSetGray = false;
		}

		//ESC�L�[�Ŕ�����
		if (input.Triger(DIK_ESCAPE))
		{
			break;
		}

		// �`�揈��
		posteffect->preDrawScene(directx.cmdList.Get(), directx.dev.Get());
		//�w�i�X�v���C�g�A
		//gamescene.drawBack();
		scene->nowscene->drawBack();
		//�[�x�o�b�t�@�N���A
		posteffect->depthClear(directx.cmdList.Get());
		//3D�I�u�W�F�N�g
		//gamescene.draw3D();
		scene->nowscene->draw3D();
		posteffect->postDrawScene(directx.cmdList.Get(), &directx);
		directx.preDraw();
		//�|�X�g�G�t�F�N�g
		if (isSetGray)
		{
			posteffect->setPipelineGray(directx.cmdList.Get());
		}
		else
		{
			posteffect->reSetPipeline();
		}
		posteffect->Draw(directx.cmdList.Get(), directx.dev.Get());
		//�O�i�X�v���C�g
		//gamescene.draw2D();
		scene->nowscene->draw2D();
		directx.postDraw();

		directx.finishDraw();
	}

	//audio.unLoadSoundWave(&gamescene.Title_bgm);
	FbxLoader::GetInstance()->Finalize();

	delete posteffect;

	primitiveScene::finalize();

	// �E�B���h�E�N���X��o�^����
	Win->Win_Deleate();

	return 0;
}
