struct VSout
{
    float2 texCoord : TEXCOORD;
    float3 fragPos : FRAG_POSITION;
    float3 noramlVec : NORMAL_VECTOR;
    float3 cameraPos : CAMERA_POSITION;
    float4 vertPos : SV_POSITION;
};

cbuffer Cbuf : register(b0)
{
    matrix modelMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
    matrix mvpMatrix;
    matrix normalMatrix;
};

VSout main(float3 pos : Position, float3 normal : Normal, float2 texCoord : TexCoord)
{
    VSout vsout;
    vsout.texCoord = texCoord;
    vsout.fragPos = mul( modelMatrix, float4(pos, 1.0f) ).xyz;
    vsout.noramlVec = mul(normalMatrix, float4(normal, 1.0f)).xyz;
    vsout.vertPos = mul(mvpMatrix, float4(pos, 1.0f));
    vsout.cameraPos =  - viewMatrix._14_24_34_44;
    
    return vsout;
}