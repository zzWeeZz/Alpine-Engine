#include "ConstBuffers.hlsli"

struct VS_OUTPUT
{
    float4 Position : SV_POSITION;
    float4 WorldPosition : POSITION;
    float3 Normal : NORMAL;
    float2 TexCoord : TEXCOORD;
    float3x3 tangentBasis : TBASIS;
};

VS_OUTPUT main(float4 pos : POSITION)
{
    VS_OUTPUT outPut;
    const float4x4 finalMatrix = mul(LightSpaceMatrix, modelSpace);
    outPut.Position = mul(finalMatrix, pos);
    return outPut;
}