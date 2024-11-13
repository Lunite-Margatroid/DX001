struct VSout
{
    float3 fragPos : FRAG_POSITION;
	float4 vertPos : SV_Position;
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

VSout main( float3 pos : POSITION)
{
    float4 tPos = mul(viewMatrix, mul(modelMatrix, float4(pos, 0.0f)));
    VSout outVS;
    outVS.fragPos = tPos.xyz;
    tPos = mul(projectionMatrix, tPos);
    outVS.vertPos = tPos.xyzz;
	return outVS;
}