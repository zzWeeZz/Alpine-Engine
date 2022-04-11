cbuffer Camera : register(b0)
{
    float4 cameraPosition;
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
    float3x3 tangentBasis : TBASIS;
};


VS_OUTPUT main(float4 inPos : POSITION, float2 inTexCoord : TEXCOORD, float3 inNormal : NORMAL, float3 inTangent : TANGENT, float3 inBitangent : BITANGENT)
{
    VS_OUTPUT output;
    inPos.w = 1.0f;
    float4x4 worldViewPorj = mul(toProjectionSpace, mul(toCameraSpace, modelSpace));
    output.Position = mul(worldViewPorj, inPos);
    output.WorldPosition = mul(modelSpace, inPos);
    output.Normal = mul((float3x3) modelSpace, inNormal);
    float3x3 tangentBasis = float3x3(inTangent, inBitangent, cross(inTangent, inBitangent));
    output.tangentBasis = mul((float3x3) modelSpace, transpose(tangentBasis));
    output.TexCoord = float2(inTexCoord.x, 1.0 - inTexCoord.y);

    return output;
}