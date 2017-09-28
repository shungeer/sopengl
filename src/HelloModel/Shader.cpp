#include "Shader.h"
#include "glad/glad.h"
#include <iostream>
#include <fstream>
#include <sstream>

using std::ifstream;
using std::stringstream;
using std::cout;
using std::endl;

CShader::CShader(const string& vertexPath, const string& fragPath)
{
	m_uiID = -1;	// 默认

	string vertexCode;
	string fragmentCode;
	ifstream vShaderFile, fShaderFile;
	vShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
	fShaderFile.exceptions(ifstream::failbit | ifstream::badbit);

	try
	{
		// 打开文件
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragPath);
		stringstream vShaderStream, fShaderStream;
		// 读取文件的缓冲内容到数据流中
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// 关闭文件处理器
		vShaderFile.close();
		fShaderFile.close();
		// 转换数据流到string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (ifstream::failure e)
	{
		cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << endl;
		return;
	}
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	// 顶点着色器
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vShaderCode, NULL);		// 附加着色器代码
	glCompileShader(vertexShader);									// 编译着色器代码
	// 检查着色器是否编译成功
	int  success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
		return;
	}
	// 片段着色器
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAG::COMPILATION_FAILED\n" << infoLog << std::endl;
		return;
	}
	// 链接到一个使用定义着色器渲染的程序
	unsigned int gShaderProgram;
	gShaderProgram = glCreateProgram();
	glAttachShader(gShaderProgram, vertexShader);	// 这个着色器的输出作为下一个定义紧邻着色器的输入
	glAttachShader(gShaderProgram, fragmentShader);
	glLinkProgram(gShaderProgram);
	// 检查着色器是否链接成功
	glGetProgramiv(gShaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(gShaderProgram, 512, NULL, infoLog);
		cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << endl;
		return;
	}
	// 删除着色器，它们已经链接到我们的程序中了，已经不再需要了
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	m_uiID = gShaderProgram;
}


CShader::~CShader()
{
	if (m_uiID != -1)
	{
		glDeleteProgram(m_uiID);
	}
}

// 激活着色器程序函数
void CShader::use()
{
	glUseProgram(m_uiID);
}
// uniform工具函数
void CShader::setBool(const string& name, bool value)
{
	glUniform1i(glGetUniformLocation(m_uiID, name.c_str()), (int)value);
}
void CShader::setInt(const string& name, int value)
{
	glUniform1i(glGetUniformLocation(m_uiID, name.c_str()), value);
}
void CShader::setFloat(const string& name, float value)
{
	glUniform1f(glGetUniformLocation(m_uiID, name.c_str()), value);
}
void CShader::setVecf3(const string& name, float v1, float v2, float v3)
{
	glUniform3f(glGetUniformLocation(m_uiID, name.c_str()), v1, v2, v3);
}
void CShader::setVecf4(const string& name, float v1, float v2, float v3, float v4)
{
	glUniform4f(glGetUniformLocation(m_uiID, name.c_str()), v1, v2, v3, v4);
}
void CShader::setVecf3(const string& name, const cell::Vec3<float>& v)
{
	glUniform3f(glGetUniformLocation(m_uiID, name.c_str()), v.x(), v.y(), v.z());
}
void CShader::setVecf4(const string& name, const cell::Vec4<float>& v)
{
	glUniform4f(glGetUniformLocation(m_uiID, name.c_str()), v.x(), v.y(), v.z(), v.w());
}
void CShader::setMatf4(const string& name, const cell::Mat4<float>& transform)
{
	//glUniformMatrix4fv(glGetUniformLocation(m_uiID, name.c_str()), 1, GL_FALSE, glm::value_ptr(transform));
	glUniformMatrix4fv(glGetUniformLocation(m_uiID, name.c_str()), 1, GL_FALSE, transform.get());
}