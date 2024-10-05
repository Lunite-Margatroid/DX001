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
    float3 cameraPos;
};

Texture2D normalBuffer : register(t4);

// linear sample, clamp to edge. 
SamplerState samNormalBuffer : register(s2);

VSout main(float3 pos : Postion, float2 texCoord : TexCoord)
{
    VSout vsout;
    vsout.texCoord = texCoord;
    vsout.fragPos = mul(modelMatrix, float4(pos, 1.0f));
    vsout.normalVec = normalBuffer.SampleLevel(samNormalBuffer, texCoord, 0.0f).xyz;
    vsout.vertPos = mul(mvpMatrix, float4(pos, 1.0f));
    vsout.cameraPos = cameraPos;
    
	return vsout;
}