#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

#define NRPOINTLIGHTS 4

struct DirLight	// �����Դ
{
	vec3 direction;	// �����ԭ��ָ���Դ;����ռ�ο�ϵ
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
struct PointLight	// ���Դ
{
	vec3 position;	// ����ռ�ο�ϵ
	// ��ԴӰ�����
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	// ��Դ˥��ϵ��
	float constant;
    float linear;
    float quadratic;
};
struct SpotLight	// �۹��Դ
{
	vec3 position;	// λ��
	vec3 direction;	// �����ԭ��ָ���Դ;����ռ�ο�ϵ	
	float cutOff;	// ���й�ǣ���Ϊ��Ҫ�ȽϽǶȴ�С�����㷴���Һ����ܣ�������ֱ�Ӵ洢�Ƕȵ�����ֵ��
	float outCutOff;	// ���й��
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

// ����ڹ۲�����ϵ��
out vec3 Normal;
out vec3 FragPos;
out vec3 ViewPos;
out vec2 TexCoords;
out vec3 vDirLightdirection;	
out vec3 vDirLightambient;	
out vec3 vDirLightdiffuse;	
out vec3 vDirLightspecular;
	
out vec3 vPtLight0position;
out vec3 vPtLight0ambient;
out vec3 vPtLight0diffuse;
out vec3 vPtLight0specular;	
out float vPtLight0constant;
out float vPtLight0linear;
out float vPtLight0quadratic;
out vec3 vPtLight1position;
out vec3 vPtLight1ambient;
out vec3 vPtLight1diffuse;
out vec3 vPtLight1specular;	
out float vPtLight1constant;
out float vPtLight1linear;
out float vPtLight1quadratic;
out vec3 vPtLight2position;
out vec3 vPtLight2ambient;
out vec3 vPtLight2diffuse;
out vec3 vPtLight2specular;	
out float vPtLight2constant;
out float vPtLight2linear;
out float vPtLight2quadratic;
out vec3 vPtLight3position;
out vec3 vPtLight3ambient;
out vec3 vPtLight3diffuse;
out vec3 vPtLight3specular;	
out float vPtLight3constant;
out float vPtLight3linear;
out float vPtLight3quadratic;

out vec3 vSpotLightposition;	
out vec3 vSpotLightdirection;	
out vec3 vSpotLightambient;	
out vec3 vSpotLightdiffuse;	
out vec3 vSpotLightspecular;
out float vSpotLightcutOff;
out float vSpotLightoutCutOff;

// ����ռ�����ϵ�µ�������λ��
uniform vec3 viewPos;
uniform DirLight dirLight;	
uniform PointLight ptLight[NRPOINTLIGHTS];	
uniform SpotLight spotLight;

// �任����		
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	// ת�����۲�����ϵ��
	Normal = mat3(transpose(inverse(view*model))) * aNormal;	// ������
	FragPos = vec3(view * model * vec4(aPos, 1.0));				// ƬԪ����
	ViewPos = vec3(view * vec4(viewPos, 1.0));					// �ӵ�����
	TexCoords = aTexCoords;										// ��������
	// �����
	vDirLightdirection = vec3(view * vec4(dirLight.direction, 1.0));	
	vDirLightambient = dirLight.ambient;
	vDirLightdiffuse = dirLight.diffuse;
	vDirLightspecular = dirLight.specular;
	// ���Դ
	vPtLight0position = vec3(view * vec4(ptLight[0].position, 1.0));
	vPtLight0ambient = ptLight[0].ambient;
	vPtLight0diffuse = ptLight[0].diffuse;
	vPtLight0specular = ptLight[0].specular;
	vPtLight0constant = ptLight[0].constant;
	vPtLight0linear = ptLight[0].linear;
	vPtLight0quadratic = ptLight[0].quadratic;
			 
	vPtLight1position = vec3(view * vec4(ptLight[1].position, 1.0));
	vPtLight1ambient = ptLight[1].ambient;
	vPtLight1diffuse = ptLight[1].diffuse;
	vPtLight1specular = ptLight[1].specular;
	vPtLight1constant = ptLight[1].constant;
	vPtLight1linear = ptLight[1].linear;
	vPtLight1quadratic = ptLight[1].quadratic;
			 
	vPtLight2position = vec3(view * vec4(ptLight[2].position, 1.0));
	vPtLight2ambient = ptLight[2].ambient;
	vPtLight2diffuse = ptLight[2].diffuse;
	vPtLight2specular = ptLight[2].specular;
	vPtLight2constant = ptLight[2].constant;
	vPtLight2linear = ptLight[2].linear;
	vPtLight2quadratic = ptLight[2].quadratic;
			 
	vPtLight3position = vec3(view * vec4(ptLight[3].position, 1.0));
	vPtLight3ambient = ptLight[3].ambient;
	vPtLight3diffuse = ptLight[3].diffuse;
	vPtLight3specular = ptLight[3].specular;
	vPtLight3constant = ptLight[3].constant;
	vPtLight3linear = ptLight[3].linear;
	vPtLight3quadratic = ptLight[3].quadratic;
	// �۹��
	vSpotLightposition = vec3(view * vec4(spotLight.position, 1.0));	
	vSpotLightdirection = vec3(view * vec4(spotLight.direction, 1.0));	
	vSpotLightambient = spotLight.ambient;
	vSpotLightdiffuse = spotLight.diffuse;
	vSpotLightspecular = spotLight.specular;
	vSpotLightcutOff = spotLight.cutOff;
	vSpotLightoutCutOff = spotLight.outCutOff;
}
