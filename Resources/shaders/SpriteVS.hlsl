#include "Sprite.hlsli"

VSOutput main( float4 pos : POSITION , float2 uv:TEXCORD)
{
	VSOutput output;
	output.svpos = mul(mat, pos);
	output.uv = uv;
	return output;
}