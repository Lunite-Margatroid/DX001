struct VSout
{
	float2  texCoord : TEXCOORD;
	float4	vertPos : SV_POSITION;
};

cbuffer Cbuf
{
	matrix modelMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
	matrix mvpMatrix;
	matrix normalMatrix;
};

VSout main(float3 pos : Position, float2 texCoord : TexCoord)
{
	VSout vsout;
	vsout.texCoord = texCoord;
	vsout.vertPos = mul(mvpMatrix, float4(pos, 1.0f));
	return vsout;
}