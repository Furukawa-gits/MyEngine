#include "PostEffectBace.hlsli"

Texture2D<float4> tex0 : register(t0);
Texture2D<float4> tex1 : register(t1);
SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
	float4 texcolor0 = tex0.Sample(smp,input.uv);

	float4 col = texcolor0;

	float v = col.x * 0.298912 + col.y * 0.586611 + col.z * 0.114478;

	return float4(v, v, v, col.w);
}