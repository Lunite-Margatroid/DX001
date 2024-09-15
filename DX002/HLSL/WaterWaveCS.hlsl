
// the settings for updating water wave
cbuffer WaterWaveSettings : register(b0)
{
    float waveConstant0;
    float waveConstant1;
    float waveConstant2;
    
    float gridStep;
};

RWTexture2D<float> preGrid : register(u0);
RWTexture2D<float> curGrid : register(u1);
RWTexture2D<float> outputGrid : register(u2);


[numthreads(16, 16, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
    uint x = DTid.x;
    uint y = DTid.y;
    
    outputGrid[uint2(x, y)].x =
        waveConstant0 * curGrid[uint2(x, y)].x +
        waveConstant1 * (curGrid[uint2(x - 1, y)].x + curGrid[uint2(x + 1, y)].x + curGrid[uint2(x, y - 1)].x + curGrid[uint2(x, y + 1)].x) +
        waveConstant2 * preGrid[uint2(x, y)].x;

}