struct VSout
{
    float2 texCoord : TEXCOORD;
    float3 fragPos : FRAG_POSITION;
    float3 normalVec : NORMAL_VECTOR;
    float3 cameraPos : CAMERA_POSITION;
    float4 vertPos : SV_POSITION;
};

cbuffer Cbuf : register(b0)
{
    matrix modelMatrix;
    matrix viewMatrix;
    matrix projectcionMatrix;
    matrix mvpMatrix;
    matrix normalMatrix;
    float3 cameraPos;
};


VSout main(float4 pos : Position, float4 normal: Normal, float2 texCoord : TexCoord)
{
    VSout vsout;
    vsout.texCoord = texCoord;
    vsout.fragPos = mul(modelMatrix, float4(pos.xyz, 1.0f));
    vsout.normalVec = normalize(mul(normalMatrix, float4(normal.xyz, 0.0f)).xyz);
    vsout.vertPos = mul(mvpMatrix, float4(pos.xyz, 1.0f));
    vsout.cameraPos = cameraPos;
    
	return vsout;
}