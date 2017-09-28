#version 330 core
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
}; 
struct Light {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    // ˥��ϵ��
    float constant;
    float linear;
    float quadratic;
};

out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;
in vec3 LightPos;   // �����Ǵ�Ƭ��ָ���Դ
in vec3 SpotDir;
in float LightCutOff;
in float LightOutCutOff;
in vec3 ViewPos;

uniform Material material;
uniform Light light;

void main()
{
    // �۹���
    vec3 lightDir = normalize(LightPos - FragPos);
    vec3 norm = normalize(Normal);
    
    vec3 spotDir = normalize(SpotDir); 
    float theta = dot(spotDir, lightDir);  
    float epsilon = LightCutOff - LightOutCutOff;
    float intensity = clamp((theta - LightOutCutOff)/epsilon, 0.0, 1.0);

    vec3 result;
    if (theta > LightCutOff)    // ִ�й��ռ���
    {
        // �������
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = light.diffuse * (diff * vec3(texture(material.diffuse, TexCoords))) ;
        // ���廷����
        vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
        // ���徵���
        vec3 viewDir = normalize(ViewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        vec3 specular = light.specular * (spec * vec3(texture(material.specular, TexCoords)));
       
        // ˥��
        float distance = length(LightPos - FragPos);
        float attenuation = 1.0 / (light.constant + light.linear*distance + light.quadratic*(distance*distance));
        // �����Դ
        result = ambient + (diffuse + specular) * attenuation * intensity;
    }
    else    // ��ʾ������ 
    {
        // ���廷����
        vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
        result = ambient;
    }
    FragColor = vec4(result, 1.0);
}