#include "ConstBuffers.hlsli"

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float4 WorldPosition : POSITION;
    float3 Normal : NORMAL;
    float2 texcoord : TEXCOORD;
    float3x3 tangentBasis : TBASIS;
};

Texture2D albedoTexture : register(t0);
Texture2D roughnessTexture : register(t1);
Texture2D normalTexture : register(t2);
Texture2D aoTexture : register(t3);
Texture2D metallicTexture : register(t4);
Texture2D emissionTexture : register(t5);

TextureCube specularTexture : register(t10);
TextureCube irradianceTexture : register(t11);

Texture2D specularBRDF_LUT : register(t6);
SamplerState defaultSampler : register(s0);
SamplerState spBRDF_Sampler : register(s1);

static const float PI = 3.141592;
static const float Epsilon = 0.00001;

static const uint NumLights = 3;

// Constant normal incidence Fresnel factor for all dielectrics.
static const float3 Fdielectric = 0.04;

float ndfGGX(float cosLh, float roughness)
{
    float alpha = roughness * roughness;
    float alphaSq = alpha * alpha;

    float denom = cosLh * (alphaSq - 1.0) + 1.0;
    denom = PI * denom * denom;
    return alphaSq / max(denom, Epsilon);
}

// Single term for separable Schlick-GGX below.
float gaSchlickG1(float cosTheta, float k)
{
    return cosTheta / (cosTheta * (1.0 - k) + k);
}

// Schlick-GGX approximation of geometric attenuation function using Smith's method.
float gaSchlickGGX(float cosLi, float cosLo, float roughness)
{
    float r = roughness + 1.0;
    float k = (r * r) / 8.0; // Epic suggests using this roughness remapping for analytic lights.
    return gaSchlickG1(cosLi, k) * gaSchlickG1(cosLo, k);
}

// Shlick's approximation of the Fresnel factor.
float3 fresnelSchlick(float3 F0, float cosTheta)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

// Returns number of mipmap levels for specular IBL environment map.
uint querySpecularTextureLevels()
{
    uint width, height, levels;
    specularTexture.GetDimensions(0, width, height, levels);
    return levels;
}

// PBR Directional light function
float3 CalcDirectionalLight(float3 viewDirection, float3 normal, float3 albedo, float roughness, float metallness, float3 fs)
{
    float3 halfVector = normalize(DirLightDirection.xyz + viewDirection);
    float NdotL = saturate(dot(normal, DirLightDirection.xyz));
    float NdotH = saturate(dot(normal, halfVector));
    float NdotV = saturate(dot(normal, viewDirection));
    float VdotH = saturate(dot(viewDirection, halfVector));

    float dist = ndfGGX(NdotH, roughness);
    float geoSmith = gaSchlickGGX(NdotL, NdotV, roughness);
    float3 F0 = fresnelSchlick(fs, VdotH);


    float3 KD = (float3(1, 1, 1) - F0) * (1 - metallness);

    float3 specular = (dist * geoSmith * F0) / max(4.0 * NdotL * NdotV, Epsilon);
    float3 Diffuse = KD * albedo;

    return ((Diffuse / PI + specular) * NdotL) * DirLightColor.xyz;
}

float3 CalcPointLight(float3 viewDirection, float3 pos, float3 normal, float3 albedo, float roughness, float metallness, float3 fs)
{
    float3 finalColor = 0.0f;
    for (int i = 0; i < 8; i++)
    {
        float distFromPixel = length(pointLights[i].position.xyz - pos);
        if (distFromPixel > pointLights[i].radius)
        {
            finalColor += float3(0, 0, 0);
            continue;
        }

        float3 dirToLight = normalize(pointLights[i].position.xyz - pos);
        float3 halfVector = normalize(dirToLight + (viewDirection));

        float attenuation = clamp(1 - distFromPixel / pointLights[i].radius, 0.f, 1.f);
        attenuation *= attenuation;

        attenuation *= lerp(attenuation, 1.f, pointLights[i].fallOff);

        float3 radiance = (pointLights[i].color.xyz * pointLights[i].color.w) * attenuation;

        float NdotL = saturate(dot(normal, dirToLight));
        float NdotH = saturate(dot(normal, halfVector));
        float NdotV = saturate(dot(normal, viewDirection));
        float VdotH = saturate(dot(viewDirection, halfVector));

        float dist = ndfGGX(NdotH, roughness);
        float geoSmith = gaSchlickGGX(NdotL, NdotV, roughness);
        float3 F0 = fresnelSchlick(fs, VdotH);

        float3 KD = lerp(float3(0, 0, 0),float3(1, 1, 1) - F0, metallness);
        float3 specular = (F0 * dist * geoSmith) / 4.0 * NdotV * NdotL;
        float3 Diffuse = KD * albedo;

        finalColor += ((Diffuse / PI + specular) * radiance) * NdotL;
    }
    return finalColor;
}

// PBR SpotLight light function
float3 CalcSpotLight(float3 viewDirection, float3 normal, float3 albedo, float roughness)
{
    float3 finalColor = 0.0f;
    for (int i = 0; i < 8; i++)
    {
        float3 halfVector = normalize(spotLights[i].position.xyz + viewDirection);
        float NdotL = saturate(dot(normal, spotLights[i].position.xyz));
        float NdotH = saturate(dot(normal, halfVector));
        float NdotV = saturate(dot(normal, viewDirection));
        float VdotH = saturate(dot(viewDirection, halfVector));

        float3 F = fresnelSchlick(float3(0.04, 0.04, 0.04), VdotH);
        float3 D = F * (NdotL / 4.0);
        float3 G = gaSchlickGGX(NdotV, NdotH, roughness);
        float3 H = F * G * VdotH;

        float3 F_R = fresnelSchlick(float3(0.04, 0.04, 0.04), VdotH);
        float3 D_R = F_R * (NdotL / 4.0);
        float3 G_R = gaSchlickGGX(NdotV, NdotH, roughness);
        float3 H_R = F_R * G_R * VdotH;

        float3 F_M = fresnelSchlick(float3(0.04, 0.04, 0.04), VdotH);
        float3 D_M = F_M * (NdotL / 4.0);
        float3 G_M = gaSchlickGGX(NdotV, NdotH, roughness);
        float3 H_M = F_M * G_M * VdotH;

        float3 F_E = fresnelSchlick(float3(0.04, 0.04, 0.04), VdotH);
        float3 D_E = F_E * (NdotL / 4.0);
        float3 G_E = gaSchlickGGX(NdotV, NdotH, roughness);
        float3 H_E = F_E * G_E * VdotH;

        float3 diffuse = albedo * NdotL;
        float3 specular = albedo * H;
        float3 metalic = albedo * H_M;
        float3 emission = albedo * H_E;
        finalColor += (diffuse + specular + metalic + emission) * spotLights[i].color.xyz;
    }
    return finalColor;
}

//Frostbite accurate SRGB to linear conversion
float3 SRGBToLinear(in float3 color)
{
    float3 linearRGBLo = color / 12.92f;
    float3 linearRGBHi = pow((color + 0.055f) / 1.055f, 2.4f);
    float3 linearRGB = (color <= 0.04045f) ? linearRGBLo : linearRGBHi;

    return linearRGB;
}

float3 LinearToSRGB(in float3 color)
{
    float3 sRGBLo = color * 12.92f;
    float3 sRGBHi = pow(abs(color), 1.f / 2.4f) * 1.055f - 0.055f;
    float3 sRGB = (color <= 0.0031308f) ? sRGBLo : sRGBHi;

    return sRGB;
}

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

// Pixel shader
float4 main(VS_OUTPUT pin) :SV_Target
{
	// Sample input textures to get shading model params.
    float3 albedo = SRGBToLinear(albedoTexture.Sample(defaultSampler, pin.texcoord).rgb);
    float alpha = albedoTexture.Sample(defaultSampler, pin.texcoord).a;
    float metalness = metallicTexture.Sample(defaultSampler, pin.texcoord).r;
    float roughness = roughnessTexture.Sample(defaultSampler, pin.texcoord).r;

	// Outgoing light direction (vector from world-space fragment position to the "eye").
    float3 Lo = normalize(cameraPosition.xyz - pin.WorldPosition.xyz);

	// Get current fragment's normal and transform to world space.
    float3 N = normalize(2.0 * normalTexture.Sample(defaultSampler, pin.texcoord).rgb - 1.0);
    N = normalize(mul(pin.tangentBasis, N));
	
	// Angle between surface normal and outgoing light direction.
    float cosLo = max(0.0, dot(N, Lo));
		
	// Specular reflection vector.
    float3 Lr = 2.0 * cosLo * N - Lo;

	// Fresnel reflectance at normal incidence (for metals use albedo color).
    float3 F0 = lerp(Fdielectric, albedo, metalness);

	// Direct lighting calculation for analytical lights.
    float3 directLighting = 0.0f;
    directLighting += CalcDirectionalLight(Lo, N, albedo, roughness, metalness, F0);
    directLighting += CalcPointLight(Lo, pin.WorldPosition.xyz, N, albedo, roughness, metalness, F0);
    //directLighting += CalcSpotLight(Lo, N, albedo, roughness);

	// Ambient lighting (IBL).
    float3 ambientLighting;
	{
		// Sample diffuse irradiance at normal direction.
        float3 irradiance = irradianceTexture.SampleLevel(defaultSampler, N, 0).rgb;

		// Calculate Fresnel term for ambient lighting.
		//https://seblagarde.wordpress.com/2011/08/17/hello-world/
        float3 F = fresnelSchlick(F0, cosLo);

		// Get diffuse contribution factor (as with direct lighting).
        float3 kd = lerp(1.0 - F, 0.0, metalness);

		// Irradiance map contains exitant radiance assuming Lambertian BRDF, no need to scale by 1/PI here either.
        float3 diffuseIBL = kd * albedo * irradiance;

		// Sample pre-filtered specular reflection environment at correct mipmap level.
        uint specularTextureLevels = querySpecularTextureLevels();
        float3 specularIrradiance = specularTexture.SampleLevel(defaultSampler, Lr, roughness * specularTextureLevels).rgb;

		// Split-sum approximation factors for Cook-Torrance specular BRDF.
        float2 specularBRDF = specularBRDF_LUT.Sample(spBRDF_Sampler, float2(cosLo, roughness)).rg;

		// Total specular IBL contribution.
        float3 specularIBL = (F * specularBRDF.x + specularBRDF.y) * specularIrradiance;

		// Total ambient lighting contribution.
        ambientLighting = diffuseIBL + specularIBL;
    }
    float4 finalColor = 0.0f;
    finalColor = float4(directLighting + ambientLighting,alpha);
    finalColor.xyz = LinearToSRGB(finalColor.xyz);
    finalColor.xyz = ACESTonemap(finalColor.xyz);
	// Final fragment color.
    return finalColor;
}