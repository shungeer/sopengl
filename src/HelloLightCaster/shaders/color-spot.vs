#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

struct SpotLight	// 聚光光源
{
	vec3 position;	// 位置
	vec3 direction;	// 方向
	float cutOff;	// 内切光角（因为需要比较角度大小，计算反余弦耗性能，故这里直接存储角度的余弦值）
	float outCutOff;	// 外切光角
};

out vec3 Normal;
out vec3 FragPos;
out vec3 LightPos;
out vec3 SpotDir;
out float LightCutOff;
out float LightOutCutOff;
out vec3 ViewPos;
out vec2 TexCoords;

// 世界空间坐标系下的向量和位置
uniform vec3 viewPos;
uniform SpotLight spot;	

// 变换矩阵		
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	// 转换到观察坐标系下
	Normal = mat3(transpose(inverse(view*model))) * aNormal;
	FragPos = vec3(view * model * vec4(aPos, 1.0));
	LightPos = vec3(view * vec4(spot.position, 1.0));
	SpotDir = vec3(view * vec4(spot.direction, 1.0));
	LightCutOff = spot.cutOff;
	LightOutCutOff = spot.outCutOff;
	ViewPos = vec3(view * vec4(viewPos, 1.0));
	TexCoords = aTexCoords;
}