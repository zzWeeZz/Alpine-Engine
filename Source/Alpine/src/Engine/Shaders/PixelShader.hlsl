struct VS_OUTPUT
{
    float4 Position : SV_POSITION;
    float4 WorldPosition : POSITION;
    float3 Normal : NORMAL;
    float2 TexCoord : TEXCOORD;
};
cbuffer Camera : register(b0)
{
    float4 cameraPosition;
    float4x4 toCameraSpace;
    float4x4 toProjectionSpace;
};
cbuffer LightBuffer : register(b2)
{
    struct DirectionalLight
    {
        float4 LightColor;
        float4 LightDirection;
    } DirLights[4];
    float4 ambientColor;
};

Texture2D ObjTexture;
SamplerState ObjSamplerState;

float4 main(VS_OUTPUT input, float3 normal : NORMAL) : SV_TARGET
{
    float4 textureColor = ObjTexture.Sample(ObjSamplerState, input.TexCoord);
    clip(textureColor.a - .25f);
    float4 dir = -DirLights[0].LightDirection;
    float4 l = normalize(dir);
    float4 n = normalize(float4(input.Normal, 1.0f));
    float4 dotp = max(dot(l, n), 0.0f);
    float4 diffuse = DirLights[0].LightColor * dotp;
    float4 CameraDir = input.WorldPosition - cameraPosition;
    float4 normalizeCameraDir = normalize(CameraDir);
    float4 reflection = reflect(l, n);
    float4 spec = DirLights[0].LightColor * pow(saturate(dot(reflection, normalizeCameraDir)), DirLights[0].LightColor.w);

    float4 final = ambientColor + (spec + diffuse) * textureColor;
    return saturate(final);
}