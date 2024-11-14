struct VSout
{
    float3 texVec : TEX_VEC;
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

VSout main( float3 pos : Position)
{
    float4 tPos = mul(viewMatrix, float4(pos, 0.0f));
    VSout outVS;
    outVS.texVec = pos;
    tPos = mul(projectionMatrix, float4(tPos.xyz, 1.0f));
    outVS.vertPos = tPos.xyww;
	return outVS;
}