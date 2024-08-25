struct VSout
{
    float2 texcoord : TEXCOORD;
    float4 position : SV_POSITION;
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


VSout main( float2 pos : Position , float2 txd : TexCoord)
{
    VSout vsout;
    vsout.texcoord = txd;
    vsout.position = mul(mvpMatrix, float4(pos, 0.0f, 1.0f));
    
	return vsout;
}