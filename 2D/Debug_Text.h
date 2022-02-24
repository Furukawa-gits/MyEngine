#pragma once
#include"SpriteCommon.h"
#include"SpriteSingleunit.h"
#include"../Base/TexManager.h"


class DebugText
{
public:
	static const int MaxCharCount = 1024;
	static const int FontWid = 9;
	static const int FontHei = 18;
	static const int FontLineCount = 14;

	DebugText();
	~DebugText();

	void Init(ID3D12Device* dev, int window_wid, int window_hei, UINT texnumber, TexManager* tex);

	void Print(const SpriteCommon& spritecommon, TexManager* tex, const std::string& text, float x, float y, float scale = 1.0f);

	void DrawAll(ID3D12GraphicsCommandList* cmdlist, const SpriteCommon& spritecommon, TexManager* tex, ID3D12Device* dev);

private:
	//スプライト配列
	SingleSprite sprites[MaxCharCount];

	//スプライト配列の添え字
	int spriteIndex = 0;
};