RWTexture2D<float> preGrid : register(u0);
RWTexture2D<float> curGrid : register(u1);

[numthreads(16, 16, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
    preGrid[DTid.xy] = 0.0f;
    curGrid[DTid.xy] = 0.0f;
}