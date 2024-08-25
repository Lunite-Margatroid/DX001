Texture2D tex1 : register(t0);
Texture2D tex2 : register(t1);

RWTexture2D<unorm float4> g_output : register(u0) ;

[numthreads(16, 16, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
    g_output[DTid.xy] = tex1[DTid.xy] * tex2[DTid.xy];
}