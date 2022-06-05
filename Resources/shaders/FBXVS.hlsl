#include"FBX.hlsli"

VSOutput main(VSInput input)
{
	SkinOutput skinned = ComputeSkin(input);
	float4 wnormal = normalize(nul(world, float4(skinned.normal, 0)));
	VSOutput output;
	output.svpos = mul(mul(viewproj, world), skinned.pos);
	output.normal = wnormal.xyz;
	output.uv = input.uv;
	return output;
}

struct SkinOutput
{
	float4 pos;
	float3 normal;
};

Skinoutput ComputeSkin(VSInput input)
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