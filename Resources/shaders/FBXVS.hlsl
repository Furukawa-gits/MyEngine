#include"FBX.hlsli"

struct SkinOutput
{
	float4 pos;
	float3 normal;
};

SkinOutput ComputeSkin(VSInput input)
{
	SkinOutput output = (SkinOutput)0;

	uint ibone;
	float weight;
	matrix m;

	ibone = input.boneIndices.x;
	weight = input.boneWeights.x;
	m = matSkinning[ibone];
	output.pos += weight * mul(m, input.pos);
	output.normal += weight * mul((float3x3)m, input.normal);

	ibone = input.boneIndices.y;
	weight = input.boneWeights.y;
	m = matSkinning[ibone];
	output.pos += weight * mul(m, input.pos);
	output.normal += weight * mul((float3x3)m, input.normal);

	ibone = input.boneIndices.z;
	weight = input.boneWeights.z;
	m = matSkinning[ibone];
	output.pos += weight * mul(m, input.pos);
	output.normal += weight * mul((float3x3)m, input.normal);

	ibone = input.boneIndices.w;
	weight = input.boneWeights.w;
	m = matSkinning[ibone];
	output.pos += weight * mul(m, input.pos);
	output.normal += weight * mul((float3x3)m, input.normal);

	return output;
}

VSOutput main(VSInput input)
{
	SkinOutput skinned = ComputeSkin(input);

	float4 wnormal = normalize(mul(world, float4(skinned.normal, 0)));
	float4 wpos = mul(world, input.pos);

	VSOutput output;

	output.svpos = mul(mul(viewproj, world), skinned.pos);
	output.normal = wnormal.xyz;
	output.uv = input.uv;
	output.color = color;
	output.worldpos = wpos;

	return output;
}