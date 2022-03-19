
cbuffer ConstantBufferObject
{
    float4x4 worldViewPosition;
};

Texture2D ObjTexture;
SamplerState ObjSamplerState;

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float2 TexCoord : TEXTCOORD;
};


VS_OUTPUT VS(float4 inPos : POSITION, float2 inTexCoord : TEXCOORD)
{
    VS_OUTPUT output;

    output.Pos = mul(inPos, worldViewPosition);
    output.TexCoord = inTexCoord;

    return output;
}

float4 PS(VS_OUTPUT input) : SV_TARGET
{
    float4 diffuse = ObjTexture.Sample(ObjSamplerState, input.TexCoord);
    clip(diffuse.a - .25f);
    return diffuse;
}