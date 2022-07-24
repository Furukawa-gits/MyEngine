#pragma once
#include"SpriteSingleunit.h"


class DebugText
{
public:
	static const int MaxCharCount = 1024;
	static const int FontWid = 9;
	static const int FontHei = 18;
	static const int FontLineCount = 14;

	DebugText();
	~DebugText();

	void Init();

	void Print(const std::string& text, float x, float y, float scale = 1.0f);

	void DrawAll(ID3D12GraphicsCommandList* cmdlist);

private:
	//�X�v���C�g�z��
	SingleSprite sprites[MaxCharCount];

	//�X�v���C�g�z��̓Y����
	int spriteIndex = 0;
};