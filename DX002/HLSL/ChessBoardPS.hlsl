// combine with LightedMaterialedVS

Texture2D texDiffuse;
Texture2D texSpecular;
SamplerState smp;

static const int LIGHT_MAX_COUNT = 16;

cbuffer Shiniess : register(b1)
{
    float shininess;
};

struct DirLight
{
    float3 Color;
    float3 Contribution;
    float3 Direction;
};

struct PointLight
{
    float3 Color;
    float3 Contribution;
    float3 Position;
    
    float kConstant;
    float kLinear;
    float kQuadratic;
};

struct SpotLight
{
    float3 Color;
    float3 Contribution;
    float3 Position;
    float3 Direction;
    
    float outerEdge;
    float innerEdge;
    
    float kConstant;
    float kLinear;
    float kQuadratic;
};

cbuffer DirLights : register(b2)
{
    int dSize;
    DirLight dLights[LIGHT_MAX_COUNT];
};

cbuffer PointLights : register(b3)
{
    int pSize;
    PointLight pLights[LIGHT_MAX_COUNT];
};

cbuffer SpotLights : register(b4)
{
    int sSize;
    SpotLight sLights[LIGHT_MAX_COUNT];
};

void CalcLight(in float3 fragPos, in float3 normalVec, in float3 cameraPos,  
            out float3 ambient, out float3 diffuse, out float3 specular)
{
    ambient = float3(0.0f, 0.0f, 0.0f);
    diffuse = float3(0.0f, 0.0f, 0.0f);
    specular = float3(0.0f, 0.0f, 0.0f);
    
    float3 viewDir = normalize(cameraPos - fragPos);

    // direction light
    int i = 0;
    for (i = 0; i < dSize;i++)
    {
        // ambient
        ambient += dLights[i].Color * dLights[i].Contribution.x;
        // diffuse
        diffuse += max(dot(normalVec, -dLights[i].Direction), 0.0f) * dLights[i].Color * dLights[i].Contribution.y;
        // specular
        specular += pow(
            max( dot(normalize(reflect(dLights[i].Direction, normalVec)), viewDir), 0.0f ), shininess)
            * dLights[i].Color * dLights[i].Contribution.z;
    }
    
    // Point light
    for (i = 0; i < pSize;i++)
    {
        float3 direction = pLights[i].Position - fragPos;
        float dist2 = dot(direction, direction);
        float dist = sqrt(dist2);
        direction = direction / dist;
        float k = pLights[i].kConstant + pLights[i].kLinear * dist + pLights[i].kQuadratic * dist2;
        
        float3 tempColor = pLights[i].Color / k;
        
        // ambient
        ambient += tempColor * pLights[i].Contribution.x;
        // diffuse
        diffuse += max(dot(normalVec, direction), 0.0f) * tempColor * pLights[i].Contribution.y;
        // specular
        specular += pow(max(dot(reflect(-direction, normalVec), viewDir), 0.0f), shininess) *
                    tempColor * pLights[i].Contribution.z;
    }
    
    // spot light
    for (i = 0; i < sSize;i++)
    {
        float3 direction = sLights[i].Position - fragPos;
        float dist2 = dot(direction, direction);
        float dist = sqrt(dist2);
        direction = direction / dist;
        float k = sLights[i].kConstant + sLights[i].kLinear * dist + sLights[i].kQuadratic * dist2;
        float transition = (dot(-direction, sLights[i].Direction) - sLights[i].outerEdge) / (sLights[i].innerEdge - sLights[i].outerEdge);
        transition = clamp(transition, 0.0f, 1.0f);
        
        float3 tempColor = sLights[i].Color * transition / k;
        
        // ambient
        ambient += tempColor * sLights[i].Contribution.x;
        // diffuse
        diffuse += max(dot(normalVec, direction), 0.0f) * tempColor * sLights[i].Contribution.y;
        // specular
        specular += pow(max(dot(reflect(-direction, normalVec), viewDir), 0.0f), shininess) / k * tempColor * sLights[i].Contribution.z;
    }
}

float4 ChessBoardFrag(in float2 texCoord)
{
    float u = frac(texCoord.x) - 0.5f;
    float v = frac(texCoord.y) - 0.5f;
    
    float uv = u * v;
    if (uv > 0)
        return float4(1.0f, 1.0f, 1.0f, 1.0f);
    return float4(0.0f, 0.0f, 0.0f, 1.0f);

}

float4 main(float2 texCoord : TEXCOORD, float3 fragPos : FRAG_POSITION, float3 normalVec : NORMAL_VECTOR, float3 cameraPos : CAMERA_POSITION) : SV_TARGET
{
    float3 ambient;
    float3 diffuse;
    float3 specular;
    
    CalcLight(fragPos, normalVec, cameraPos, ambient, diffuse, specular);
    
    const float3 grayCalc = float3(0.299f, 0.587f, 0.114f);
    
    return float4((ambient + diffuse), 1.0f) * ChessBoardFrag(texCoord * 8)
            + float4(specular, dot(grayCalc, specular)) * texSpecular.Sample(smp, texCoord);
}