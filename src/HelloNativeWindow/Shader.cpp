#include "Shader.h"
#include "glad/glad.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <fstream>
#include <sstream>

using std::ifstream;
using std::stringstream;
using std::cout;
using std::endl;

CShader::CShader(const string& vertexPath, const string& fragPath)
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


CShader::~CShader()
{
	if (m_uiID != -1)
	{
		glDeleteProgram(m_uiID);
	}
}

// ������ɫ��������
void CShader::use()
{
	glUseProgram(m_uiID);
}
// uniform���ߺ���
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
void CShader::setVecf4(const string& name, float v1, float v2, float v3, float v4)
{
	glUniform4f(glGetUniformLocation(m_uiID, name.c_str()), v1, v2, v3, v4);
}
void CShader::setMat4(const string& name, const glm::mat4& transform)
{
	glUniformMatrix4fv(glGetUniformLocation(m_uiID, name.c_str()), 1, GL_FALSE, glm::value_ptr(transform));
}