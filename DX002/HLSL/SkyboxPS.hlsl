TextureCube tex : register(t0);
SamplerState smpCube : register(s0);

float4 main(float3  fragPos: FRAG_POSITION) : SV_TARGET
{   
    return tex.Sample(smpCube, fragPos);
}