#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

struct SpotLight	// �۹��Դ
{
	vec3 position;	// λ��
	vec3 direction;	// ����
	float cutOff;	// ���й�ǣ���Ϊ��Ҫ�ȽϽǶȴ�С�����㷴���Һ����ܣ�������ֱ�Ӵ洢�Ƕȵ�����ֵ��
	float outCutOff;	// ���й��
};

out vec3 Normal;
out vec3 FragPos;
out vec3 LightPos;
out vec3 SpotDir;
out float LightCutOff;
out float LightOutCutOff;
out vec3 ViewPos;
out vec2 TexCoords;

// ����ռ�����ϵ�µ�������λ��
uniform vec3 viewPos;
uniform SpotLight spot;	

// �任����		
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	// ת�����۲�����ϵ��
	Normal = mat3(transpose(inverse(view*model))) * aNormal;
	FragPos = vec3(view * model * vec4(aPos, 1.0));
	LightPos = vec3(view * vec4(spot.position, 1.0));
	SpotDir = vec3(view * vec4(spot.direction, 1.0));
	LightCutOff = spot.cutOff;
	LightOutCutOff = spot.outCutOff;
	ViewPos = vec3(view * vec4(viewPos, 1.0));
	TexCoords = aTexCoords;
}