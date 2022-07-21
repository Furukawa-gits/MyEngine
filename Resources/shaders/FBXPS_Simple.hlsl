#include"FBX.hlsli"

Texture2D<float4> tex : register(t0); // 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0); // 0番スロットに設定されたサンプラー

struct PSOutput
{
	float4 target0: SV_TARGET0;
	float4 target1: SV_TARGET1;
};

PSOutput main(VSOutput input)
{
	PSOutput output;

	output.target0 = float4(0, 1, 0, 1.0f);
	output.target1 = float4(0, 1, 0, 1.0f);

	return output;
}