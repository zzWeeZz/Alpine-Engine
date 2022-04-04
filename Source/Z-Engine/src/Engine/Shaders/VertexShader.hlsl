cbuffer ConstantBufferObject
{
    float4x4 modelSpace;
    float4x4 toCameraSpace;
    float4x4 toProjectionSpace;
};

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float2 TexCoord : TEXCOORD;
};


VS_OUTPUT main(float4 inPos : POSITION,float2 inTexCoord : TEXCOORD)
{
    VS_OUTPUT output;
    inPos.w = 1.0f;
    float4x4 worldViewPorj = mul(toProjectionSpace, mul(toCameraSpace, modelSpace));
    output.Pos = mul(worldViewPorj, inPos);
    output.TexCoord = inTexCoord;

    return output;
}