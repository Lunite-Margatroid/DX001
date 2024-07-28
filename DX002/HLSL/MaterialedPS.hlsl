Texture2D texDiffuse;
Texture2D texSpecular;
SamplerState smp;


float4 main(float2 texCoord : TEXCOORD) : SV_TARGET
{
    return texDiffuse.Sample(smp, texCoord);
}