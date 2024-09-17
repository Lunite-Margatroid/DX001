
// the settings for updating water wave
cbuffer WaterWaveDisturb : register(b0)
{
    uint2 disturbCoord;
    float disturbHeight;
};

RWTexture2D<float> outputGrid : register(u2);


[numthreads(1, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    uint x = disturbCoord.x;
    uint y = disturbCoord.y;
    
    float halfHaight = disturbHeight * 0.5f;
    
    outputGrid[uint2(x, y)] += disturbHeight;
    outputGrid[uint2(x + 1, y)] += halfHaight;
    outputGrid[uint2(x - 1, y)] += halfHaight;
    outputGrid[uint2(x, y + 1)] += halfHaight;
    outputGrid[uint2(x, y - 1)] += halfHaight;

}