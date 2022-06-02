struct PixelShaderInput
{
    float3 localPosition : POSITION;
    float4 pixelPosition : SV_POSITION;
};

TextureCube environmentMap : register(t0);
SamplerState defaultSampler : register(s0);

float3 ACESTonemap(float3 color)
{
    float3x3 m1 = float3x3(
		0.59719, 0.07600, 0.02840,
		0.35458, 0.90834, 0.13383,
		0.04823, 0.01566, 0.83777
	);
    float3x3 m2 = float3x3(
		1.60475, -0.10208, -0.00327,
		-0.53108, 1.10813, -0.07276,
		-0.07367, -0.00605, 1.07602
	);
    float3 v = mul(color, m1);
    float3 a = v * (v + 0.0245786) - 0.000090537;
    float3 b = v * (0.983729 * v + 0.4329510) + 0.238081;
    return clamp(mul((a / b), m2), 0.0, 1.0);
}
float4 main(PixelShaderInput pixelInput) : SV_TARGET
{
    float3 envVector = normalize(pixelInput.localPosition);
    return float4(ACESTonemap(environmentMap.SampleLevel(defaultSampler, envVector, 0).xyz), 1);
}