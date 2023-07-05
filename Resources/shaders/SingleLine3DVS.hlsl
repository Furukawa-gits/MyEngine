#include "SingleLine3D.hlsli"

VSOutput main(float4 Pos : POSITION)
{
	VSOutput output;
	output.svpos = mul(projMat, Pos);
	return output;
}