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
};

out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;
in vec3 LightDir;   // 方向是从片段指向光源
in vec3 ViewPos;

uniform Material material;
uniform Light light;

void main()
{
    // 定向光照
    // 方向约定从光源发出
    vec3 lightDir = normalize(-LightDir);   // 取反
    // 漫反射光
    vec3 norm = normalize(Normal);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * vec3(texture(material.diffuse, TexCoords))) ;
    // 物体环境光
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    // 物体镜面光
    vec3 viewDir = normalize(ViewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * vec3(texture(material.specular, TexCoords)));
    // 整体光源
    vec3 result = (ambient + diffuse + specular);
    FragColor = vec4(result, 1.0);
}