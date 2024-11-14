TextureCube tex : register(t0);
SamplerState smpCube : register(s0);

float4 main(float3 texVec : TEX_VEC) : SV_TARGET
{   
    return tex.Sample(smpCube, texVec);
}