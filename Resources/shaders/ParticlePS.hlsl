#include "Particle.hlsli"

Texture2D<float4> tex : register(t0); // 0番テクスチャ
SamplerState smp : register(s0); // 0番サンプラー

float4 main(GSOutput input) : SV_Target
{
    float4 texColor = tex.Sample(smp, input.uv);
    return texColor * input.color;
}