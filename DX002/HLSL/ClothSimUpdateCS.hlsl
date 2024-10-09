RWTexture2D<float4> velRecord : register(u0);
RWTexture2D<float4> preGrid : register(u1);
RWBuffer<float4> curGrid : register(u2);


cbuffer ClothSimSetting : register(b0)
{
    float deltaTime; // cycle time of a frame
    uint nIteration; // num of iterator
    float alpha; // the elastic factor of spring
    float m; // the mass of the particle
    
    float height; // height of the cell
    float width; // widtd of the cell
    uint nHeight; // 
    uint nWidth; // num of vertex of the gird's width
    
    float G; // gravitational acceleration
    float tAlpha; // tAlpha = alpha / (deltaTime ^ 2)
    float tBeta;
    
    uint constraintU;
    
    uint constraintV;
    float constraintX;
    float constraintY;
    float constraintZ; // it equal sqrt( height ^ 2 + width ^ 2 ) when update the vertex position
};

[numthreads(16, 16, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    uint u = DTid.x;
    uint v = DTid.y;
    float3 vel = velRecord[uint2(u, v)].xyz;
    float3 preX = preGrid[uint2(u, v)].xyz;
    
    float gridWidth[3];
    gridWidth[0] = height;
    gridWidth[1] = width;
    gridWidth[2] = constraintZ;
    
    // initial
    
    float lambda[8];
    lambda[0] = 0.0f;
    lambda[1] = 0.0f;
    lambda[2] = 0.0f;
    lambda[3] = 0.0f;
    lambda[4] = 0.0f;
    lambda[5] = 0.0f;
    lambda[6] = 0.0f;
    lambda[7] = 0.0f;
    
    int2 offset[8];
    offset[0] = int2(0, 1);
    offset[1] = int2(1, 0);
    offset[2] = int2(0, -1);
    offset[3] = int2(-1, 0);
    
    offset[4] = int2( 1, -1);
    offset[5] = int2(-1, -1);
    offset[6] = int2(-1,  1);
    offset[7] = int2( 1,  1);
    
    // neibor vertex
    float3 neiborVertex[8];
    for (int i = 0; i < 8;i ++)
    {
        int uu = int(u) + offset[i].x;
        int vv = int(v) + offset[i].y;
        if (uu < 0 || uu >= nWidth || vv < 0 || vv >= nHeight)
            neiborVertex[i] = preX;
        else
            neiborVertex[i] = preGrid[uint2(uu, vv)].xyz;
    }
    
    
    // calc the accaleration
    float3 a = float3(0.0f, -G, 0.0f);
    for (int j = 0; j < 8; j++)
    {
        int uu = int(u) + offset[j].x;
        int vv = int(v) + offset[j].y;
        if (uu < 0 || uu >= nWidth || vv < 0 || vv >= nHeight)
            continue;
        float3 p = preX - neiborVertex[j];
        float dist = length(p);
        float3 gridC = float3(0.0f, 0.0f, 0.0f);
        if (dist > 0.0001f)
            gridC = p / dist;
        float C = dist - gridWidth[2];
        if (j < 4)
            C = dist - gridWidth[j % 2];
        
        a += -C * gridC / alpha / m;
    }
    
    float3 newX = preX + vel + 0.5f * deltaTime * deltaTime * a;

    for (int i = 0; i < nIteration; i++)
    {
        float3 deltaX = float3(0.0f, 0.0f, 0.0f);
        for (int j = 0; j < 8; j++)
        {
            int uu = int(u) + offset[j].x;
            int vv = int(v) + offset[j].y;
            if (uu < 0 || uu >= nWidth || vv < 0 || vv >= nHeight)
                continue;
            float3 p = newX - neiborVertex[j];
            float dist = length(p);
            float3 gridC  = float3(0.0f, 0.0f, 0.0f);
            if (dist > 0.0001f)
               gridC = p / dist;
            float C = dist - gridWidth[2];
            if (j < 4)
                C = dist - gridWidth[j % 2];
            float deltaLambda = (-C - tAlpha * lambda[j]) / (dot(gridC, gridC) / m + tAlpha);
            
            lambda[j] += deltaLambda;
            deltaX += gridC * deltaLambda;
        }
        deltaX /= m;
        newX += deltaX;
    }
    
    curGrid[u + v * nWidth] = float4(newX, 1.0f);
    velRecord[uint2(u, v)] = float4(newX - preX, 1.0f);
    
}