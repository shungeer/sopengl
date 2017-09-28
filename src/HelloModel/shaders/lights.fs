#version 330 core
#define NRPOINTLIGHTS 4
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
}; 
struct DirLight// �����Դ
{
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
struct PointLight// ���Դ
{
    vec3 position;
    // ��ԴӰ�����
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    // ��Դ˥��ϵ��
    float constant;
    float linear;
    float quadratic;
};
struct SpotLight    // �۹��Դ
{
    vec3 position; // λ��
    vec3 direction;
    float cutOff;   // ���й�ǣ���Ϊ��Ҫ�ȽϽǶȴ�С�����㷴���Һ����ܣ�������ֱ�Ӵ洢�Ƕȵ�����ֵ��
    float outCutOff;    // ���й��
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
    // �������
    DirLight dirLight;
    dirLight.direction = vDirLightdirection;
    dirLight.ambient = vDirLightambient;
    dirLight.diffuse = vDirLightdiffuse;
    dirLight.specular = vDirLightspecular;

    // ���Դ
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

    // �۹��
    SpotLight spotLight;
    spotLight.position = vSpotLightposition;
    spotLight.direction = vSpotLightdirection;
    spotLight.ambient = vSpotLightambient;
    spotLight.diffuse = vSpotLightdiffuse;
    spotLight.specular = vSpotLightspecular;
    spotLight.cutOff = vSpotLightcutOff;
    spotLight.outCutOff = vSpotLightoutCutOff;
    // ����
    vec3 normal = normalize(Normal);
    vec3 viewDir = normalize(ViewPos - FragPos);

    vec3 result;
    // ��һ�׶� �������
    result = CalcDirLight(dirLight, normal, viewDir);
    // �ڶ��׶� ���Դ
    for (int i=0; i<NRPOINTLIGHTS; i++)
    {
        result += CalcPointLight(ptLight[i], normal, FragPos, viewDir);
    }
    // �����׶� �۹���
    result += CalcSpotLight(spotLight, normal, FragPos, viewDir);
    FragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    // ��������
    vec3 lightDir = normalize(-light.direction);
    // �������
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;
    // ������
    vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;
    // ���淴���
    vec3 reflectDir = reflect(-lightDir, normal); // ���շ���Լ��Ϊ��Դλ�÷���
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;

    return diffuse + ambient + specular;
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    // ������Դ
    // ���շ�����ƬԪָ���Դλ��
    vec3 lightDir = normalize(light.position - fragPos);
    // �������
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;
    // ������
    vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;
    // ���淴���
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(reflectDir, viewDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;
    // ˥��������
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);

    return (diffuse + ambient + specular) * attenuation;
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    // ����۹��
    // ���շ�����ƬԪָ���Դλ��
    vec3 lightDir = normalize(light.position - fragPos);
    // �������
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;
    // ������
    vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;
    // ���淴���
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(reflectDir, viewDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;
    // ����׵��ƽ��Ч��
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = (light.cutOff - light.outCutOff);
    float intensity = clamp((theta - light.outCutOff)/epsilon, 0.0, 1.0);

    return ambient + (diffuse + specular) * intensity;
}