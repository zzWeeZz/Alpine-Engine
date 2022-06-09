#include "ConstBuffers.hlsli"

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float4 WorldPosition : POSITION;
    float3 Normal : NORMAL;
    float2 texcoord : TEXCOORD;
    float3x3 tangentBasis : TBASIS;
};

float4 main(VS_OUTPUT vOutput) : SV_Target
{
	return float4(0, 0, 0, 0);
}