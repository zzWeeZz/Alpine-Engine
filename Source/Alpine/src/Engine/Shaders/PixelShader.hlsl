struct VS_OUTPUT
{
    float4 Position : SV_POSITION;
    float4 WorldPosition : POSITION;
    float3 Normal : NORMAL;
    float2 TexCoord : TEXCOORD;
};

cbuffer LightBuffer : register(b2)
{
    struct DirectionalLight
    {
        float4 LightColor;
        float4 LightDirection;
    } DirLights[16];
    float4 ambientColor;
};

Texture2D ObjTexture;
SamplerState ObjSamplerState;

float4 main(VS_OUTPUT input, float3 normal : NORMAL) : SV_TARGET
{
    float4 diffuse = ObjTexture.Sample(ObjSamplerState, input.TexCoord);
    clip(diffuse.a - .25f);
    float4 dir = -DirLights[0].LightDirection;
    float4 l = normalize(dir);
    float4 n = normalize(float4(input.Normal, 1.0f));
    float4 dotp = max(dot(l, n), 0.1f);
    float4 color = (diffuse * DirLights[0].LightColor);
    float4 result = color * dotp;
    float4 final = result * ambientColor;
    return saturate(final);
}