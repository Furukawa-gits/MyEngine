#include"Debug_Text.h"

DebugText::DebugText()
{
}

DebugText::~DebugText()
{
}

void DebugText::Init(ID3D12Device* dev, int window_wid, int window_hei, UINT texnumber,TexManager* tex)
{
	for (int i = 0; i < _countof(sprites); i++)
	{
		sprites[i].GenerateSprite(dev, window_wid, window_hei, texnumber, tex, false, false, false, false);
	}
}

void DebugText::Print(const SpriteCommon& spritecommon,TexManager* tex, const std::string& text, float x, float y, float scale)
{
	for (int i = 0; i < text.size(); i++)
	{
		if (spriteIndex >= MaxCharCount)
		{
			break;
		}

		const unsigned char& character = text[i];

		int fontIndex = character - 32;
		if (character >= 0x7f)
		{
			fontIndex = 0;
		}

		int fontindexY = fontIndex / FontLineCount;
		int fontindexX = fontIndex % FontLineCount;

		sprites[spriteIndex].anchorpoint = { 0,0 };
		sprites[spriteIndex].position = { x + FontWid * scale * i,y,0 };
		sprites[spriteIndex].texLeftTop = { (float)fontindexX * FontWid,(float)fontindexY * FontHei };
		sprites[spriteIndex].texSize = { FontWid,FontHei };
		sprites[spriteIndex].size = { FontWid * scale,FontHei * scale };

		sprites[spriteIndex].SpriteTransferVertexBuffer(tex, true);

		sprites[spriteIndex].SpriteUpdate(spritecommon);

		spriteIndex++;
	}
}

void DebugText::DrawAll(ID3D12GraphicsCommandList* cmdlist, const SpriteCommon& spritecommon, TexManager* tex, ID3D12Device* dev)
{
	for (int i = 0; i < spriteIndex; i++)
	{
		sprites[i].DrawSprite(cmdlist, tex, dev);
	}

	spriteIndex = 0;
}
