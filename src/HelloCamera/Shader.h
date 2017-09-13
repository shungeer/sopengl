#pragma once
#include <string>
#include "glm/detail/type_mat.hpp"

using std::string;
class CShader
{
public:
	CShader(const string& vertexPath, const string& fragPath);
	~CShader();

	// 激活着色器程序函数
	void use();
	// uniform工具函数
	void setBool(const string& name, bool value);
	void setInt(const string& name, int value);
	void setFloat(const string& name, float value);
	void setVecf4(const string& name, float v1, float v2, float v3, float v4);
	void setMat4(const string& name, const glm::mat4& transform);
private:
	unsigned int m_uiID;		// 着色器程序ID
};

