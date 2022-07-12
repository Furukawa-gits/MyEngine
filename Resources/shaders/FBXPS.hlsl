#include"FBX.hlsli"

Texture2D<float4> tex : register(t0); // 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0); // 0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

struct PSOutput
{
	float4 target0: SV_TARGET0;
	float4 target1: SV_TARGET1;
};

PSOutput main(VSOutput input)
{
	PSOutput output;

	float4 texcolor = tex.Sample(smp, input.uv);

	float3 light = normalize(float3(1, -1, 1));
	float diffuse = saturate(dot(-light, input.normal));
	float brightness = diffuse + 0.3f;
	float4 shadercolor = float4(brightness, brightness, brightness, 1.0f);

	output.target0 = shadercolor * texcolor;
	output.target1 = shadercolor * texcolor;
	//output.target1 = float4(1 - (shadercolor * texcolor).rgb, 1);

	return output;
}