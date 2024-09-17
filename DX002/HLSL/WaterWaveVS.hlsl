struct VSout
{
    float2 texCoord : TEXCOORD;
    float3 fragPos : FRAG_POSITION;
    float3 noramlVec : NORMAL_VECTOR;
    float3 cameraPos : CAMERA_POSITION;
    float4 vertPos : SV_POSITION;
};

cbuffer Cbuf : register(b0)
{
    matrix modelMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
    matrix mvpMatrix;
    matrix normalMatrix;
    float3 cameraPos;
};

cbuffer WaterWaveSettings : register(b2)
{
    float waveConstant0;
    float waveConstant1;
    float waveConstant2;
    
    float gridStep;
};

// liner sampler, clamp to border, border = 0
SamplerState samHeightTex : register(s1);

// height texture
Texture2D texHeight : register(t3);

VSout main(float3 pos : Position, float3 normal : Normal, float2 texCoord : TexCoord)
{
    VSout vsout;
    vsout.texCoord = texCoord;
    // vsout.noramlVec = normalize(mul(normalMatrix, float4(normal, 0.0f)).xyz);
    vsout.vertPos = float4(pos, 1.0f);
    vsout.cameraPos = cameraPos;
    
    // sample height texture and calculate normal vec
    // vsout.vertPos.y += texHeight.SampleLevel(samHeightTex, vsout.texCoord, 0.0f).r;
    float left = texHeight.SampleLevel(samHeightTex, vsout.texCoord, 0.0f, int2(-1, 0)).r;
    float right = texHeight.SampleLevel(samHeightTex, vsout.texCoord, 0.0f, int2(1, 0)).r;
    float top = texHeight.SampleLevel(samHeightTex, vsout.texCoord, 0.0f, int2(0, -1)).r;
    float bottom = texHeight.SampleLevel(samHeightTex, vsout.texCoord, 0.0f, int2(0, 1)).r;
    
    vsout.noramlVec = normalize(
        mul(normalMatrix, float4(-right + left, 2.0f * gridStep, bottom - top, 0.0f)).xyz);
    
    // vsout.noramlVec = normalize(mul(normalMatrix, float4(normal, 0.0f)).xyz);
    
    // vsout.noramlVec = float3(0.0, 1.0f, 0.0);
    
    vsout.fragPos = mul(modelMatrix, vsout.vertPos).xyz;
    vsout.vertPos = mul(mvpMatrix, vsout.vertPos);
    
    
    
    return vsout;
}