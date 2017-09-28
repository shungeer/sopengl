#version 330 core
#define NRPOINTLIGHTS 4
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
}; 
struct DirLight// 定向光源
{
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
struct PointLight// 点光源
{
    vec3 position;
    // 光源影响分量
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    // 光源衰减系数
    float constant;
    float linear;
    float quadratic;
};
struct SpotLight    // 聚光光源
{
    vec3 position; // 位置
    vec3 direction;
    float cutOff;   // 内切光角（因为需要比较角度大小，计算反余弦耗性能，故这里直接存储角度的余弦值）
    float outCutOff;    // 外切光角
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec3 ViewPos;
in vec2 TexCoords;

in vec3 vDirLightdirection;
in vec3 vDirLightambient;
in vec3 vDirLightdiffuse;
in vec3 vDirLightspecular;
                 
in vec3 vPtLight0position;
in vec3 vPtLight0ambient;
in vec3 vPtLight0diffuse;
in vec3 vPtLight0specular;
in float vPtLight0constant;
in float vPtLight0linear;
in float vPtLight0quadratic;
in vec3 vPtLight1position;
in vec3 vPtLight1ambient;
in vec3 vPtLight1diffuse;
in vec3 vPtLight1specular;
in float vPtLight1constant;
in float vPtLight1linear;
in float vPtLight1quadratic;
in vec3 vPtLight2position;
in vec3 vPtLight2ambient;
in vec3 vPtLight2diffuse;
in vec3 vPtLight2specular;
in float vPtLight2constant;
in float vPtLight2linear;
in float vPtLight2quadratic;
in vec3 vPtLight3position;
in vec3 vPtLight3ambient;
in vec3 vPtLight3diffuse;
in vec3 vPtLight3specular;
in float vPtLight3constant;
in float vPtLight3linear;
in float vPtLight3quadratic;

in vec3 vSpotLightposition;
in vec3 vSpotLightdirection;
in vec3 vSpotLightambient;
in vec3 vSpotLightdiffuse;
in vec3 vSpotLightspecular;
in float vSpotLightcutOff;
in float vSpotLightoutCutOff;

uniform Material material;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{   
    // 定向光照
    DirLight dirLight;
    dirLight.direction = vDirLightdirection;
    dirLight.ambient = vDirLightambient;
    dirLight.diffuse = vDirLightdiffuse;
    dirLight.specular = vDirLightspecular;

    // 点光源
    PointLight ptLight[NRPOINTLIGHTS];
    ptLight[0].position = vPtLight0position;
    ptLight[0].ambient = vPtLight0ambient;
    ptLight[0].diffuse = vPtLight0diffuse;
    ptLight[0].specular = vPtLight0specular;
    ptLight[0].constant = vPtLight0constant;
    ptLight[0].linear = vPtLight0linear;
    ptLight[0].quadratic = vPtLight0quadratic;

    ptLight[1].position = vPtLight1position;
    ptLight[1].ambient = vPtLight1ambient;
    ptLight[1].diffuse = vPtLight1diffuse;
    ptLight[1].specular = vPtLight1specular;
    ptLight[1].constant = vPtLight1constant;
    ptLight[1].linear = vPtLight1linear;
    ptLight[1].quadratic = vPtLight1quadratic;

    ptLight[2].position = vPtLight2position;
    ptLight[2].ambient = vPtLight2ambient;
    ptLight[2].diffuse = vPtLight2diffuse;
    ptLight[2].specular = vPtLight2specular;
    ptLight[2].constant = vPtLight2constant;
    ptLight[2].linear = vPtLight2linear;
    ptLight[2].quadratic = vPtLight2quadratic;

    ptLight[3].position = vPtLight3position;
    ptLight[3].ambient = vPtLight3ambient;
    ptLight[3].diffuse = vPtLight3diffuse;
    ptLight[3].specular = vPtLight3specular;
    ptLight[3].constant = vPtLight3constant;
    ptLight[3].linear = vPtLight3linear;
    ptLight[3].quadratic = vPtLight3quadratic;

    // 聚光灯
    SpotLight spotLight;
    spotLight.position = vSpotLightposition;
    spotLight.direction = vSpotLightdirection;
    spotLight.ambient = vSpotLightambient;
    spotLight.diffuse = vSpotLightdiffuse;
    spotLight.specular = vSpotLightspecular;
    spotLight.cutOff = vSpotLightcutOff;
    spotLight.outCutOff = vSpotLightoutCutOff;
    // 属性
    vec3 normal = normalize(Normal);
    vec3 viewDir = normalize(ViewPos - FragPos);

    vec3 result;
    // 第一阶段 定向光照
    result = CalcDirLight(dirLight, normal, viewDir);
    // 第二阶段 点光源
    for (int i=0; i<NRPOINTLIGHTS; i++)
    {
        result += CalcPointLight(ptLight[i], normal, FragPos, viewDir);
    }
    // 第三阶段 聚光照
    result += CalcSpotLight(spotLight, normal, FragPos, viewDir);
    FragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    // 定向光计算
    vec3 lightDir = normalize(-light.direction);
    // 漫反射光
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;
    // 环境光
    vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;
    // 镜面反射光
    vec3 reflectDir = reflect(-lightDir, normal); // 光照方向约定为光源位置发出
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;

    return diffuse + ambient + specular;
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    // 计算点光源
    // 光照方向由片元指向光源位置
    vec3 lightDir = normalize(light.position - fragPos);
    // 漫反射光
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;
    // 环境光
    vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;
    // 镜面反射光
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(reflectDir, viewDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;
    // 衰减量计算
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);

    return (diffuse + ambient + specular) * attenuation;
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    // 计算聚光灯
    // 光照方向由片元指向光源位置
    vec3 lightDir = normalize(light.position - fragPos);
    // 漫反射光
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;
    // 环境光
    vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;
    // 镜面反射光
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(reflectDir, viewDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;
    // 光照椎体平滑效果
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = (light.cutOff - light.outCutOff);
    float intensity = clamp((theta - light.outCutOff)/epsilon, 0.0, 1.0);

    return ambient + (diffuse + specular) * intensity;
}