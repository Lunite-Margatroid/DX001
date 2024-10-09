RWBuffer<float4> normalGrid : register(u3);
RWBuffer<float4> curGrid : register(u2);
RWTexture2D<float4> preGrid : register(u1);

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
    
    preGrid[uint2(u, v)] = curGrid[u + v * nWidth];
    
    if (u >= nWidth - 1)
        u = nWidth - 2;
    if (v >= nHeight - 1)
        v = nHeight - 2;
    
    uint idx = u + v * nWidth;
    
    float3 curVertex = curGrid[idx].xyz;
    float3 right = curGrid[idx + 1].xyz;
    float3 bottom = curGrid[idx + nWidth].xyz;
    
    normalGrid[DTid.y * nWidth + DTid.x] = float4(normalize(cross(bottom - curVertex, right - curVertex)), 1.0f);
}