RWTexture2D<float3> normalGrid : register(u3);
RWTexture2D<float3> curGrid : register(u2);

cbuffer ClothSimSetting : register(b0)
{
    float deltaTime; // cycle time of a frame
    uint nIteration; // num of iterator
    float alpha; // the elastic factor of spring
    float m; // the mass of the particle
    
    float height; // height of the grid
    float width; // widtd of the grid
    uint nHeight; // cell height
    uint nWidth; // cell width
    
    float G; // gravitational acceleration
    float tAlpha; // tAlpha = alpha / (deltaTime ^ 2)
    float tBeta;
    
    uint constraintU;
    uint constraintV;
    float constraintX;
    float constraintY;
    float constraintZ;
};


[numthreads(16, 16, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
    int u = DTid.x;
    int v = DTid.y;
    
    if (u >= nWidth - 1)
        u = nWidth - 2;
    if (v >= nHeight - 1)
        v = nHeight - 2;
    
    float3 curVertex = curGrid[uint2(u, v)];
    float3 right = curGrid[uint2(u + 1, v)];
    float3 bottom = curGrid[uint2(u, v + 1)];
    
    normalGrid[DTid.xy] = normalize(cross(bottom - curVertex, right - curVertex));

}