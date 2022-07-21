#include "PostEffectBace.hlsli"

Texture2D<float4> tex0 : register(t0);
Texture2D<float4> tex1 : register(t1);
SamplerState smp : register(s0);

float Gaussian(float2 drawUV, float2 pickUV, float sigma)
{
	float d = distance(drawUV, pickUV);
	return exp(-(d * d) / (2 * sigma * sigma));
}

float4 main(VSOutput input) : SV_TARGET
{
	float totalWeight = 0;
	float _Sigma = 0.005;
	float _StepWidth = 0.001;
	float4 texcolor0 = tex0.Sample(smp,input.uv);
	float4 col = texcolor0;

	for (float py = -_Sigma * 2; py <= _Sigma * 2; py += _StepWidth)
	{
		for (float px = -_Sigma * 2; px <= _Sigma * 2; px += _StepWidth)
		{
			float2 pickUV = input.uv + float2(px, py);
			float weight = Gaussian(input.uv, pickUV, _Sigma);
			col *= (texcolor0 + weight);
			totalWeight += weight;
		}
	}

	col.x = col.x / totalWeight;
	col.y = col.y / totalWeight;
	col.z = col.z / totalWeight;

	return col;
}