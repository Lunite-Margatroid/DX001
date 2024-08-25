Texture2D texDiffuse;
Texture2D texSpecular;
cbuffer Shininess : register(b1)
{
    float shininess;
};

SamplerState smp;

float4 main(float2 texCoord : TEXCOORD) : SV_TARGET
{
    return float4(texDiffuse.Sample(smp, texCoord).xyz, 1.0f);
}