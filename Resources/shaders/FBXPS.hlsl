#include"FBX.hlsli"

Texture2D<float4> tex : register(t0); // 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0); // 0番スロットに設定されたサンプラー

struct PSOutput
{
	float4 target0: SV_TARGET0;
	float4 target1: SV_TARGET1;
};

PSOutput main(VSOutput input) : SV_TARGET
{
	PSOutput output;

	float4 texcolor = tex.Sample(smp, input.uv);

	float3 light = normalize(float3(1, -1, 1));
	float diffuse = saturate(dot(lightv, input.normal));

	float brightness = diffuse + 0.5f;

	float4 shadercolor = float4(brightness, brightness, brightness, 1.0f);

	output.target0 = shadercolor * texcolor * input.color;
	output.target1 = shadercolor * texcolor * input.color;

	return output;
}