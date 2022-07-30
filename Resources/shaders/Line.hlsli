cbuffer cbuff0:register(b0)
{
	float4 color;
	matrix startMat;
	matrix endMat;
};

struct VSOutput
{
	float4 svpos:SV_POSITION;
};