RWTexture2D<float3> velRecord : register(u0);
RWTexture2D<float3> preGrid : register(u1);
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
void main(uint3 DTid : SV_DispatchThreadID)
{
    uint u = DTid.x;
    uint v = DTid.y;
    float3 vel = velRecord[uint2(u, v)];
    float3 preX = preGrid[uint2(u, v)];
    // gravity
    vel += deltaTime * float3(0.0f, G, 0.0f);
    
    float gridWidth[2];
    gridWidth[0] = height;
    gridWidth[1] = width;
    
   
    
    // initial
    float3 newX = vel * deltaTime + preX;
    float lambda[4];
    lambda[0] = 0.0f;
    lambda[1] = 0.0f;
    lambda[2] = 0.0f;
    lambda[3] = 0.0f;
    
    uint2 offset[4];
    offset[0] = uint2(0, 1);
    offset[1] = uint2(1, 0);
    offset[2] = uint2(0, -1);
    offset[3] = uint2(-1, 0);

    for (int i = 0; i < nIteration; i++)
    {
        float3 deltaX = float3(0.0f, 0.0f, 0.0f);
        for (int j = 0; j < 4; j++)
        {
            int uu = int(u) + offset[j].x;
            int vv = int(v) + offset[j].y;
            if (uu < 0 || uu >= nWidth || vv < 0 || vv >= nHeight)
                continue;
            float3 p = newX - curGrid[uint2(uu, vv)];
            float dist = length(p);
            float gridC = p / dist;
            float C = dist - gridWidth[j % 2];
            float deltaLambda = (-C - tAlpha * lambda[j]) / (dot(gridC, gridC) / m + tAlpha);
            
            lambda[j] += deltaLambda;
            deltaX += gridC * deltaLambda;
        }
        deltaX /= m;
        newX += deltaX;
    }
    
    curGrid[uint2(u, v)] = newX;
    velRecord[uint2(u, v)] = (newX - preX) / deltaTime;
    
}