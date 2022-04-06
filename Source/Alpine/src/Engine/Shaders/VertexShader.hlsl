cbuffer Camera : register(b0)
{
    float4x4 toCameraSpace;
    float4x4 toProjectionSpace;
};
cbuffer Model : register(b1)
{
    float4x4 modelSpace;
};

struct VS_OUTPUT
{
    float4 Position : SV_POSITION;
    float4 WorldPosition : POSITION;
    float3 Normal : NORMAL;
    float2 TexCoord : TEXCOORD;
};


VS_OUTPUT main(float4 inPos : POSITION,float2 inTexCoord : TEXCOORD, float3 inNormal : NORMAL)
{
    VS_OUTPUT output;
    inPos.w = 1.0f;
    float4x4 worldViewPorj = mul(toProjectionSpace, mul(toCameraSpace, modelSpace));
    output.Position = mul(worldViewPorj, inPos);
    output.WorldPosition = mul(modelSpace, inPos);
    output.Normal = mul((float3x3) modelSpace, inNormal);
    output.TexCoord = inTexCoord;

    return output;
}