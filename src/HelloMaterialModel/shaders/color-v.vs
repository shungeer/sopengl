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
	// ����任
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	// �����������任���۲�����ϵ(���÷��߱任����)
	vec3 normal = mat3(transpose(inverse(view * model))) * aNormal;
	vec3 norm = normalize(normal);	// ��׼��
	// ������λ��ת�����۲�����ϵ��
	vec3 FragPos = vec3(view * model * vec4(aPos, 1.0));
	// ����Դλ��ת�����۲�����ϵ��
	vec3 LightPos = vec3(view * vec4(lightPos, 1.0));;

	// ���ռ���(�۲�����ϵ)
	vec3 lightDir = normalize(LightPos - FragPos);	// ���߷���
	vec3 viewDir = normalize(viewPos - FragPos);		// �۲췽��
	// �������
	float diff = max(dot(lightDir, norm), 0.0);
	vec3 diffuse = diff * lightColor;
	// ������
	float ambientStrength = 0.2f;
    vec3 ambient = ambientStrength * lightColor;
	// �����
	float specularStrength = 1.0;
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(reflectDir, viewDir), 0.0), 64);
	vec3 specular = specularStrength * spec * lightColor;
	// ���Ϸ���
	fragColor = (diffuse + ambient + specular) * objectColor;
}