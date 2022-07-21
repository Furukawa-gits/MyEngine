#include "PostEffectBace.hlsli"

Texture2D<float4> tex0 : register(t0);
Texture2D<float4> tex1 : register(t1);
SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
	float4 texcolor0 = tex0.Sample(smp,input.uv);

	float4 col = texcolor0;

	return col;
}