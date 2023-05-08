#include"resultScene.h"

resultScene::resultScene()
{
	//���\�[�X�ǂݍ���
	loadResources();

	//�p�����[�^�̃Z�b�g
	setParameter();
}

void resultScene::loadResources()
{
	//���U���g���
	resultScreen[0].size = { 1280,720 };
	resultScreen[0].generateSprite("black_color.png");

	resultScreen[1].size = { 1280,300 };
	resultScreen[1].generateSprite("black_color.png");

	//�N���A�E�I�[�o�[���
	clearText.anchorpoint = { 0.5f,0.0f };
	clearText.size = { 640,100 };
	clearText.generateSprite("CLEAR_text.png");

	overText.anchorpoint = { 0.5f,0.0f };
	overText.size = { 640,100 };
	overText.generateSprite("OVER_text.png");

	//�^�C�g���{�^��
	titleButton.anchorpoint = { 0.5f,0.5f };
	titleButton.size = { 200,40 };
	titleButton.position = { 640 + 150,500,0 };
	titleButton.generateSprite("titleText.png");

	//�Z���N�g�{�^��
	selectButton.anchorpoint = { 0.5f,0.5f };
	selectButton.size = { 200,40 };
	selectButton.position = { 640 - 150,500,0 };
	selectButton.generateSprite("selectText.png");

	//����{�^��
	decisionButton.anchorpoint = { 0.5f,0.5f };
	decisionButton.size = { 150,5 };
	decisionButton.position = { 640,420,0 };
	decisionButton.generateSprite("selectIcon.png");
}

void resultScene::initialize()
{
	//���\�[�X�ǂݍ���
	loadResources();

	//�p�����[�^�̃Z�b�g
	setParameter();
}

void resultScene::setParameter()
{
	isMoveScreen = true;
	isScreenEase = true;
	isTextEase = false;
	resultScreenEase.set(easingType::easeOut, easingPattern::Quadratic, 40, 720, 0);
	titleButton.spriteUpdata();
	isSelectOrTitle = -1;
	ButtonEase_y.reSet();
	ButtonEase_x.reSet();
}

void resultScene::updata()
{
	if (isClearOrOver)
	{
		clearScene();
	}
	else
	{
		overScene();
	}
}

void resultScene::draw3D()
{
	groundPlane->Draw(directx->cmdList.Get());
	skySphere->Draw(directx->cmdList.Get());

	//�v���C���[�`��
	playerPointer->draw3D(directx);
}

void resultScene::draw2D()
{
	resultScreen[0].drawSprite(directx->cmdList.Get());
	if (isClearOrOver)
	{
		clearText.drawSprite(directx->cmdList.Get());
	}
	else
	{
		overText.drawSprite(directx->cmdList.Get());
	}
	resultScreen[1].drawSprite(directx->cmdList.Get());

	if (isMoveScreen)
	{
		return;
	}
	titleButton.drawSprite(directx->cmdList.Get());
	selectButton.drawSprite(directx->cmdList.Get());
	decisionButton.drawSprite(directx->cmdList.Get());

	mouseCursurSub.drawSprite(directx->cmdList.Get());
	mouseCursur.drawSprite(directx->cmdList.Get());
}

void resultScene::clearScene()
{
	//��ʔw�i�̃C�[�W���O
	if (isScreenEase)
	{
		resultScreen[0].position.y = resultScreenEase.easing();
		resultScreen[1].position.y = resultScreen[0].position.y;

		clearText.position.x = 640;
		clearText.position.y = resultScreen[0].position.y;
		clearText.spriteUpdata();

		overText.position.x = 640;
		overText.position.y = resultScreen[0].position.y;
		overText.spriteUpdata();

		if (!resultScreenEase.getIsActive())
		{
			isScreenEase = false;

			clearTextEase.set(easingType::easeOut, easingPattern::Quadratic, 25, 200, 300);

			isTextEase = true;
		}
	}
	else
	{
		resultScreen[0].position.y = 0;
		resultScreen[1].position.y = 0;
	}

	resultScreen[0].spriteUpdata();
	resultScreen[1].spriteUpdata();

	//CLEAR�EOVER�̃e�L�X�g�̃C�[�W���O
	if (isTextEase)
	{
		clearText.position.x = 640;
		clearText.position.y = clearTextEase.easing();
		clearText.spriteUpdata();

		overText.position.x = 640;
		overText.position.y = overTextEase.easing();
		overText.spriteUpdata();

		if (!clearTextEase.getIsActive() && !overTextEase.getIsActive())
		{
			isTextEase = false;
		}
	}

	if (!clearTextEase.getIsActive() && !overTextEase.getIsActive() && !resultScreenEase.getIsActive())
	{
		isMoveScreen = false;
	}
	//�w�i�ƃe�L�X�g�𓮂����I����Ă��Ȃ���΃{�^���̏����͖���
	if (isMoveScreen)
	{
		return;
	}

	bool isTitleButton = titleButton.isSpriteMouseInput();
	bool isSelectButton = selectButton.isSpriteMouseInput();

	titleButton.spriteUpdata();
	selectButton.spriteUpdata();

	if (!isMoveSelectIcon)
	{
		//select���
		if (titleButton.isMouseSelect && isSelectOrTitle == -1)
		{
			selectEase.set(easingType::easeOut, easingPattern::Cubic, 20, 640 - 150, 640 + 150);
			isMoveSelectIcon = true;
			isSelectOrTitle *= -1;
		}
		//title���
		else if (selectButton.isMouseSelect && isSelectOrTitle == 1)
		{
			selectEase.set(easingType::easeOut, easingPattern::Cubic, 20, 640 + 150, 640 - 150);
			isMoveSelectIcon = true;
			isSelectOrTitle *= -1;
		}
	}
	else
	{
		decisionButton.position = { selectEase.easing(),540,0 };

		if (!selectEase.getIsActive())
		{
			isMoveSelectIcon = false;
		}
	}

	//�Z���N�g�A�C�R���ʏ�g�k
	if (!isPushTitle && !isMoveSelectIcon)
	{
		resultIconSizeX -= 0.5f;

		if (resultIconSizeX <= 250)
		{
			resultIconSizeX = 200;
		}
		decisionButton.size = { resultIconSizeX,7 };
	}

	decisionButton.spriteUpdata();

	if ((input->Triger(DIK_SPACE) || isTitleButton || isSelectButton) && !isPushTitle && !isMoveSelectIcon)
	{
		ButtonEase_y.set(easingType::easeOut, easingPattern::Quadratic, 30, 7, 0);
		ButtonEase_x.set(easingType::easeOut, easingPattern::Quadratic, 30, 200, 300);
		isPushTitle = true;
	}

	if (isPushTitle)
	{
		decisionButton.size.y = ButtonEase_y.easing();
		decisionButton.size.x = ButtonEase_x.easing();
	}

	//���̃V�[������
	if (isPushTitle && !ButtonEase_y.getIsActive())
	{
		isNextScene = true;
	}
}

void resultScene::overScene()
{
	//��ʔw�i�̃C�[�W���O
	if (isScreenEase)
	{
		resultScreen[0].position.y = resultScreenEase.easing();
		resultScreen[1].position.y = resultScreen[0].position.y;

		clearText.position.x = 640;
		clearText.position.y = resultScreen[0].position.y;
		clearText.spriteUpdata();

		overText.position.x = 640;
		overText.position.y = resultScreen[0].position.y;
		overText.spriteUpdata();

		if (!resultScreenEase.getIsActive())
		{
			isScreenEase = false;

			overTextEase.set(easingType::easeOut, easingPattern::Quadratic, 25, 200, 300);

			isTextEase = true;
		}
	}
	else
	{
		resultScreen[0].position.y = 0;
		resultScreen[1].position.y = 0;
	}

	resultScreen[0].spriteUpdata();
	resultScreen[1].spriteUpdata();

	//CLEAR�EOVER�̃e�L�X�g�̃C�[�W���O
	if (isTextEase)
	{
		clearText.position.x = 640;
		clearText.position.y = clearTextEase.easing();
		clearText.spriteUpdata();

		overText.position.x = 640;
		overText.position.y = overTextEase.easing();
		overText.spriteUpdata();

		if (!clearTextEase.getIsActive() && !overTextEase.getIsActive())
		{
			isTextEase = false;
		}
	}

	if (!clearTextEase.getIsActive() && !overTextEase.getIsActive() && !resultScreenEase.getIsActive())
	{
		isMoveScreen = false;
	}
	//�w�i�ƃe�L�X�g�𓮂����I����Ă��Ȃ���΃{�^���̏����͖���
	if (isMoveScreen)
	{
		return;
	}

	bool isTitleButton = titleButton.isSpriteMouseInput();
	bool isSelectButton = selectButton.isSpriteMouseInput();

	titleButton.spriteUpdata();
	selectButton.spriteUpdata();

	if (!isMoveSelectIcon)
	{
		//select���
		if (titleButton.isMouseSelect && isSelectOrTitle == -1)
		{
			selectEase.set(easingType::easeOut, easingPattern::Cubic, 20, 640 - 150, 640 + 150);
			isMoveSelectIcon = true;
			isSelectOrTitle *= -1;
		}
		//title���
		else if (selectButton.isMouseSelect && isSelectOrTitle == 1)
		{
			selectEase.set(easingType::easeOut, easingPattern::Cubic, 20, 640 + 150, 640 - 150);
			isMoveSelectIcon = true;
			isSelectOrTitle *= -1;
		}
	}
	else
	{
		decisionButton.position = { selectEase.easing(),540,0 };

		if (!selectEase.getIsActive())
		{
			isMoveSelectIcon = false;
		}
	}

	//�Z���N�g�A�C�R���ʏ�g�k
	if (!isPushTitle && !isMoveSelectIcon)
	{
		resultIconSizeX -= 0.5f;

		if (resultIconSizeX <= 250)
		{
			resultIconSizeX = 200;
		}
		decisionButton.size = { resultIconSizeX,7 };
	}

	decisionButton.spriteUpdata();

	if ((input->Triger(DIK_SPACE) || isTitleButton || isSelectButton) && !isPushTitle && !isMoveSelectIcon)
	{
		ButtonEase_y.set(easingType::easeOut, easingPattern::Quadratic, 30, 7, 0);
		ButtonEase_x.set(easingType::easeOut, easingPattern::Quadratic, 30, 200, 300);
		isPushTitle = true;
	}

	if (isPushTitle)
	{
		decisionButton.size.y = ButtonEase_y.easing();
		decisionButton.size.x = ButtonEase_x.easing();
	}

	//���̃V�[������
	if (isPushTitle && !ButtonEase_y.getIsActive())
	{
		isNextScene = true;
	}
}