#include "Line.hlsli"

VSOutput main(float4 Pos : POSITION)
{
	VSOutput output;
	output.svpos = mul(startMat, Pos);
	return output;
}