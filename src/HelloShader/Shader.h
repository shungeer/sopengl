#pragma once
#include <string>

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
private:
	unsigned int m_uiID;		// 着色器程序ID
};

