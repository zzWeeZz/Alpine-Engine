cbuffer ConstantBufferObject
{
    float4x4 worldViewPosition;
};

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float2 TexCoord : TEXCOORD;
};


VS_OUTPUT main(float4 inPos : POSITION,float2 inTexCoord : TEXCOORD)
{
    VS_OUTPUT output;

    output.Pos = mul(inPos, worldViewPosition);
    output.TexCoord = inTexCoord;

    return output;
}