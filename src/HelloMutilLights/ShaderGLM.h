#pragma once
#include <string>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

using std::string;
class CGLMShader
{
public:
	CGLMShader(const string& vertexPath, const string& fragPath);
	~CGLMShader();

	// 激活着色器程序函数
	void use();
	// uniform工具函数
	void setBool(const string& name, bool value);
	void setInt(const string& name, int value);
	void setFloat(const string& name, float value);
	void setVecf3(const string& name, float v1, float v2, float v3);
	void setVecf4(const string& name, float v1, float v2, float v3, float v4);
	void setVecf3(const string& name, const glm::vec3& v);
	void setVecf4(const string& name, const glm::vec4& v);
	void setMatf4(const string& name, const glm::mat4& transform);
private:
	unsigned int m_uiID;		// 着色器程序ID
};

