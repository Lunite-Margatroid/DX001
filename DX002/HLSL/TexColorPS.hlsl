Texture2D texDiffuse;
Texture2D texSpecular;
SamplerState smp;

//struct VSout
//{
//    float2 texCoord : TEXCOORD;
//    float3 fragPos : FRAG_POSITION;
//    float3 noramlVec : NORMAL_VECTOR;
//    float3 cameraPos : CAMERA_POSITION;
//    float4 vertPos : SV_POSITION;
//};


float4 main(float2 texCoord : TEXCOORD) : SV_TARGET
{
    return texDiffuse.Sample(smp, texCoord);
}