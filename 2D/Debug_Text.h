#pragma once
#include"Sprite.h"


class DebugText
{
public:
	static const int MaxCharCount = 2000;
	static const int FontWid = 9;
	static const int FontHei = 18;
	static const int FontLineCount = 14;

	DebugText();
	~DebugText();

	void Init();

	void Print(const std::string& text, float x, float y, float scale = 1.0f);

	void DrawAll(ID3D12GraphicsCommandList* cmdlist);

private:
	//スプライト配列
	SingleSprite sprites[MaxCharCount];

	//スプライト配列の添え字
	int spriteIndex = 0;
};