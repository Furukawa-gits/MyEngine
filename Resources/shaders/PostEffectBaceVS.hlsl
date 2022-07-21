#include "PostEffectBace.hlsli"

VSOutput main( float4 pos : POSITION , float2 uv:TEXCORD)
{
	VSOutput output;
	output.svpos = pos;
	output.uv = uv;
	return output;
}