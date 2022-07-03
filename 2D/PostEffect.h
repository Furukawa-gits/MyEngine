#pragma once
#include"SpriteSingleunit.h"

class PostEffect :
    public SingleSprite
{
public:

    PostEffect();


    void Draw(ID3D12GraphicsCommandList* cmdList, TexManager* texture, ID3D12Device* dev);
};

