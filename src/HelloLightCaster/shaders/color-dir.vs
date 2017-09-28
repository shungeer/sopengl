#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 Normal;
out vec3 FragPos;
out vec3 LightDir;
out vec3 ViewPos;
out vec2 TexCoords;

// ����ռ�����ϵ�µ�������λ��
uniform vec3 lightDir;		// �����Ǵ�Ƭ��ָ���Դ
uniform vec3 viewPos;

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
	LightDir = vec3(view * vec4(lightDir, 1.0));
	ViewPos = vec3(view * vec4(viewPos, 1.0));
	TexCoords = aTexCoords;
}