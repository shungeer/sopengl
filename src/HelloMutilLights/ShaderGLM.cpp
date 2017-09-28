#include "ShaderGLM.h"
#include "glad/glad.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <fstream>
#include <sstream>

using std::ifstream;
using std::stringstream;
using std::cout;
using std::endl;

CGLMShader::CGLMShader(const string& vertexPath, const string& fragPath)
{
	m_uiID = -1;	// Ĭ��

	string vertexCode;
	string fragmentCode;
	ifstream vShaderFile, fShaderFile;
	vShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
	fShaderFile.exceptions(ifstream::failbit | ifstream::badbit);

	try
	{
		// ���ļ�
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragPath);
		stringstream vShaderStream, fShaderStream;
		// ��ȡ�ļ��Ļ������ݵ���������
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// �ر��ļ�������
		vShaderFile.close();
		fShaderFile.close();
		// ת����������string
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

	// ������ɫ��
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vShaderCode, NULL);		// ������ɫ������
	glCompileShader(vertexShader);									// ������ɫ������
	// �����ɫ���Ƿ����ɹ�
	int  success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
		return;
	}
	// Ƭ����ɫ��
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
	// ���ӵ�һ��ʹ�ö�����ɫ����Ⱦ�ĳ���
	unsigned int gShaderProgram;
	gShaderProgram = glCreateProgram();
	glAttachShader(gShaderProgram, vertexShader);	// �����ɫ���������Ϊ��һ�����������ɫ��������
	glAttachShader(gShaderProgram, fragmentShader);
	glLinkProgram(gShaderProgram);
	// �����ɫ���Ƿ����ӳɹ�
	glGetProgramiv(gShaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(gShaderProgram, 512, NULL, infoLog);
		cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << endl;
		return;
	}
	// ɾ����ɫ���������Ѿ����ӵ����ǵĳ������ˣ��Ѿ�������Ҫ��
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	m_uiID = gShaderProgram;
}


CGLMShader::~CGLMShader()
{
	if (m_uiID != -1)
	{
		glDeleteProgram(m_uiID);
	}
}

// ������ɫ��������
void CGLMShader::use()
{
	glUseProgram(m_uiID);
}
// uniform���ߺ���
void CGLMShader::setBool(const string& name, bool value)
{
	glUniform1i(glGetUniformLocation(m_uiID, name.c_str()), (int)value);
}
void CGLMShader::setInt(const string& name, int value)
{
	glUniform1i(glGetUniformLocation(m_uiID, name.c_str()), value);
}
void CGLMShader::setFloat(const string& name, float value)
{
	glUniform1f(glGetUniformLocation(m_uiID, name.c_str()), value);
}
void CGLMShader::setVecf3(const string& name, float v1, float v2, float v3)
{
	glUniform3f(glGetUniformLocation(m_uiID, name.c_str()), v1, v2, v3);
}
void CGLMShader::setVecf4(const string& name, float v1, float v2, float v3, float v4)
{
	glUniform4f(glGetUniformLocation(m_uiID, name.c_str()), v1, v2, v3, v4);
}
void CGLMShader::setVecf3(const string& name, const glm::vec3& v)
{
	glUniform3f(glGetUniformLocation(m_uiID, name.c_str()), v.x, v.y, v.z);
}
void CGLMShader::setVecf4(const string& name, const glm::vec4& v)
{
	glUniform4f(glGetUniformLocation(m_uiID, name.c_str()), v.x, v.y, v.z, v.w);
}
void CGLMShader::setMatf4(const string& name, const glm::mat4& transform)
{
	glUniformMatrix4fv(glGetUniformLocation(m_uiID, name.c_str()), 1, GL_FALSE, glm::value_ptr(transform));
}