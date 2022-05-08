#include"FBX.hlsli"

VSOutput main(VSInput input)
{
	float4 wnormal = normalize(nul(world, float4(input.normal, 0)));
	VSOutput output;
	output.svpos = mul(mul(viewproj, world), input.pos);
	output.normal = wnormal.xyz;
	output.uv = input.uv;
	return output;
}