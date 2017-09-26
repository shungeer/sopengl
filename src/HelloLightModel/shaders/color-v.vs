#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 fragColor;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	// 顶点变换
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	// 将法线向量变换到观察坐标系(利用法线变换矩阵)
	vec3 normal = mat3(transpose(inverse(view * model))) * aNormal;
	vec3 norm = normalize(normal);	// 标准化
	// 将顶点位置转换到观察坐标系下
	vec3 FragPos = vec3(view * model * vec4(aPos, 1.0));
	// 将光源位置转换到观察坐标系下
	vec3 LightPos = vec3(view * vec4(lightPos, 1.0));;

	// 光照计算(观察坐标系)
	vec3 lightDir = normalize(LightPos - FragPos);	// 光线方向
	vec3 viewDir = normalize(viewPos - FragPos);		// 观察方向
	// 漫反射光
	float diff = max(dot(lightDir, norm), 0.0);
	vec3 diffuse = diff * lightColor;
	// 环境光
	float ambientStrength = 0.2f;
    vec3 ambient = ambientStrength * lightColor;
	// 镜面光
	float specularStrength = 1.0;
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(reflectDir, viewDir), 0.0), 64);
	vec3 specular = specularStrength * spec * lightColor;
	// 整合分量
	fragColor = (diffuse + ambient + specular) * objectColor;
}