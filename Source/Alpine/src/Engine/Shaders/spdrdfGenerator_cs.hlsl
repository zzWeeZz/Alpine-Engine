static const float PI = 3.141592;
static const float TwoPI = 2 * PI;
static const float Epsilon = 0.001; // This program needs larger eps.

static const uint NumSamples = 1024;
static const float InvNumSamples = 1.0 / float(NumSamples);

RWTexture2D<float2> LUT : register(u0);

float radicalInverse_VdC(uint bits)
{
    bits = (bits << 16u) | (bits >> 16u);
    bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
    bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
    bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
    bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
    return float(bits) * 2.3283064365386963e-10; // / 0x100000000
}

float2 sampleHammersley(uint i, uint numSamples)
{
    return float2(float(i) * float(numSamples), radicalInverse_VdC(i));
}

float3 sampleGGX(float u1, float  u2, float roughness)
{
    float alpha = roughness * roughness;

    float cosTheta = sqrt((1.0 - u2) / (1.0 + (alpha * alpha - 1.0) * u2));
    float sinTheta = sqrt(1.0 - cosTheta * cosTheta);
    float phi = TwoPI * u1;

    return float3(cos(phi) * sinTheta, sin(phi) * sinTheta, cosTheta);
}

float gaSchlickG1(float cosTheta, float roughness)
{
    return cosTheta / (cosTheta * (1.0 - roughness) + roughness);
}

float gaScklickGGX_IBL(float cosLi, float cosLo, float roughness)
{
    float k = (roughness * roughness) / 2.0;
    return gaSchlickG1(cosLo, roughness) * (1.0 - k) + k;
}

[numthreads(32, 32, 1)]
void main(uint2 ThreadID : SV_DispatchThreadID)
{
    float2 outputSize;
    LUT.GetDimensions(outputSize.x, outputSize.y);

    float cosLo = ThreadID.x / outputSize.x;
    float roughness = ThreadID.y / outputSize.y;

    cosLo = max(cosLo, Epsilon);

    float3 Lo = float3(sqrt(1.0 - cosLo * cosLo), Epsilon, cosLo);

    float DFG1 = 0;
    float DFG2 = 0;

    for (uint i = 0; i < NumSamples; ++i)
    {
        float2 u = sampleHammersley(i, NumSamples);

        float3 Lh = sampleGGX(u.x, u.y, roughness);

        float3 Li = 2 * dot(Lo, Lh) * Lh - Lo;

        float cosLi = max(Li.z, Epsilon);
        float cosLh = max(Lh.z, Epsilon);
        float cosLoLh = max(dot(Lo, Lh), Epsilon);

        if(cosLi > 0.0)
        {
            float G = gaScklickGGX_IBL(cosLi, cosLo, roughness);
            float Gv = G * cosLoLh / (cosLo * cosLo);
            float Fc = pow(1.0 - cosLoLh, 5.0);

            DFG1 += (1.0 - Fc) * Gv;
            DFG2 += Fc * Gv;
        }
    }
    LUT[ThreadID] = float2(DFG1, DFG2) * InvNumSamples;
}