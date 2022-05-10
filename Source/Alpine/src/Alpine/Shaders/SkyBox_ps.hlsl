struct PixelShaderInput
{
    float3 localPosition : POSITION;
    float4 pixelPosition : SV_POSITION;
};

TextureCube environmentMap : register(t0);
SamplerState defaultSampler : register(s0);


float4 main(PixelShaderInput pixelInput) : SV_TARGET
{
    float3 envVector = normalize(pixelInput.localPosition);
	return environmentMap.SampleLevel(defaultSampler, envVector, 0);
}