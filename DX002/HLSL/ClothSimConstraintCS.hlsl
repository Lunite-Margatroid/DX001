
RWBuffer<float4> curGrid : register(u2);

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

[numthreads(1, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    curGrid[constraintU  +  nWidth * constraintV] = float4(constraintX, constraintY, constraintZ, 1.0f);
}