cbuffer Camera : register(b0)
{
    float4 cameraPosition;
    float4x4 toCameraSpace;
    float4x4 toProjectionSpace;
    float4x4 viewProjection;
};

cbuffer Model : register(b1)
{
    float4x4 modelSpace;
};

cbuffer DirectonalLight : register(b2)
{
    float4 DirLightDirection;
    float4 DirLightColor;
    float4x4 LightSpaceMatrix;
};

struct PointLight
{
    float4 position;
    float4 color;
    float fallOff;
    float radius;
    float2 padding;
};

cbuffer PointLight : register(b3)
{
   PointLight pointLights[8];
};

cbuffer SpotLight : register(b4)
{
    struct SpotLight
    {
        float4 position;
        float4 color;
        float4 direction;
        float4 attenuation;
        float4 cone;
    } spotLights[8];
};