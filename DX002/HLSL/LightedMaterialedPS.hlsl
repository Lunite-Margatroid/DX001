Texture2D texDiffuse;
Texture2D texSpecular;
SamplerState smp;

static const int LIGHT_MAX_COUNT = 16;

cbuffer Shininess : register(b1)
{
    float shininess;
};

// Contribution[0]  环境光贡献度
// Contribution[1]  漫反射光贡献度
// Contribution[2]  镜面反射光贡献度

struct DirLight
{
    float3 Color;       // 16byte
    float3 Contribution;// 32B
    float3 Direction;   // 48byte
};

struct PointLight
{
    float3 Color;
    float3 Contribution;
    float3 Position;    
    
    float kConstant;    // 48byte
    float kLinear;      // 52byte
    float kQuadratic;   // 56byte
    
    // 64B 对齐
};

struct SpotLight
{
    float3 Color;       // 16B
    float3 Contribution;// 32B
    float3 Position;    // 48B
    float3 Direction;   
    
    float outerEdge;    // 64B
    float innerEdge;
    
    float kConstant;    
    float kLinear;
    float kQuadratic;   //80 B
};

cbuffer DirLights : register(b2)
{
    int dSize;  // 16B
    DirLight dLights[LIGHT_MAX_COUNT];
};

cbuffer PointLights : register(b3)
{
    int pSize;  // 16B
    PointLight pLights[LIGHT_MAX_COUNT];
};

cbuffer SpotLights : register(b4)
{
    int sSize; // 16B
    SpotLight sLights[LIGHT_MAX_COUNT];
};

static float3 FragPos;
static float3 NormalVec;
static float3 CameraPos;

inline void CalcLight(out float3 ambient, out float3 diffuse, out float3 specular)
{
    ambient = float3(0.0f, 0.0f, 0.0f);
    diffuse = float3(0.0f, 0.0f, 0.0f);
    specular = float3(0.0f, 0.0f, 0.0f);
    float3 viewDir = normalize(CameraPos - FragPos);
    // Direction Light
    for (int i = 0; i < dSize;i++)
    {
        // ambient
        ambient += dLights[i].Color * dLights[i].Contribution.x;
        // diffuse
        diffuse += max(dot(NormalVec, dLights[i].Direction), 0.0f) * dLights[i].Color * dLights[i].Contribution.y;
        // specular
        specular += pow(max(dot(normalize(reflect(-dLights[i].Direction, NormalVec)), viewDir), 0.0f), shininess) * dLights[i].Color * dLights[i].Contribution.z;

    }
    // Point Light
    for (int i = 0; i < pSize;i++)
    {
        float direction = pLights[i].Position - FragPos;
        float dist = length(direction);
        direction = direction / dist;
        float k = pLights[i].kConstant + pLights[i].kLinear * dist + pLights[i].kQuadratic * dist * dist;
        
        // ambient
        ambient += pLights[i].Color * pLights[i].Contribution.x;
        // diffuse
        diffuse += max(dot(NormalVec, direction), 0.0f) * pLights[i].Color * pLights[i].Contribution.y;
        // specular
        specular += pow(max(dot(reflect(direction, NormalVec), viewDir), 0.0f), shininess)  / k * pLights[i].Color * pLights[i].Contribution.z;
        
    }
    // spot light
    for (int i = 0; i < sSize;i++)
    {
        float3 direction = sLights[i].Position - FragPos;
        float dist = length(direction);
        direction = direction / dist;
        float k = sLights[i].kConstant + sLights[i].kLinear * dist + sLights[i].kQuadratic * dist * dist;
        float transition = (dot(-direction, sLights[i].Direction) - sLights[i].outerEdge) / (sLights[i].innerEdge - sLights[i].outerEdge);
        transition = clamp(transition, 0.0f, 1.0f);
        
        float3 tempColor = sLights[i].Color * transition / k;
        
        // ambient
        ambient += tempColor *  sLights[i].Contribution.x ;
        // diffuse
        diffuse += max(dot(NormalVec, direction), 0.0f) * tempColor * sLights[i].Contribution.y;
        // specular
        specular += pow(max(dot(reflect(direction, NormalVec), viewDir), 0.0f), shininess) / k * tempColor * sLights[i].Contribution.z;

    }
    
}


float4 main(float texCoord : TEXCOORD, float3 fragPos : FRAG_POSITION, float3 normalVec : NORMAL_VECTOR, float3 cameraPos : CAMERA_POSITION) : SV_TARGET
{
    float3 ambient ;
    float3 diffuse ;
    float3 specular ;
    
    FragPos = fragPos;
    NormalVec = normalVec;
    CameraPos = cameraPos;
    
    CalcLight(ambient, diffuse, specular);
    
    return float4((ambient + diffuse), 1.0f) * texDiffuse.Sample(smp, texCoord) + float4(specular, 1.0f) * texSpecular.Sample(smp, texCoord);
}