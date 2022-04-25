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


cbuffer LightBuffer : register(b2)
{
    struct DirectionalLight
    {
        float4 radiance;
        float4 LightDirection;
        
    } DirLights[4];
    float4 ambientColor;
};