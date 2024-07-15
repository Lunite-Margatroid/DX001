Texture2D tex;
SamplerState smp;


float4 main(float2 texCoord : TEXCOORD) : SV_TARGET
{
	return tex.Sample(smp, texCoord);
}