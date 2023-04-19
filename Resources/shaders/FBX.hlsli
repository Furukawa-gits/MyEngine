static const int maxBones = 36;

cbuffer cbuff0 : register(b0)
{
	matrix viewproj;
	matrix world;
	float3 cameraPos;
	float4 color;
};

cbuffer cbuff1 : register(b1)
{
	float3 lightv;
	float3 lightcolor;
}

cbuffer skinning:register(b3)
{
	matrix matSkinning[maxBones];
};

struct VSInput
{
	float4 pos : POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
	uint4 boneIndices : BONEINDICES;
	float4 boneWeights : BONEWEIGHTS;
};

struct VSOutput
{
	float4 svpos : SV_POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
	float4 color : COLOR;
	float4 worldpos : POSITION;
};