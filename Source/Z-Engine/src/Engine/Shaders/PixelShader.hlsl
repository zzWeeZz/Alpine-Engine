struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float2 TexCoord : TEXCOORD;
};

Texture2D ObjTexture;
SamplerState ObjSamplerState;

float4 main(VS_OUTPUT input) : SV_TARGET
{
    float4 diffuse = ObjTexture.Sample(ObjSamplerState, input.TexCoord);
    clip(diffuse.a - .25f);
    diffuse.a = 0.5f;
    return diffuse;
}