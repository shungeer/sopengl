#pragma once
#include <string>

using std::string;
class CShader
{
public:
	CShader(const string& vertexPath, const string& fragPath);
	~CShader();

	// ������ɫ��������
	void use();
	// uniform���ߺ���
	void setBool(const string& name, bool value);
	void setInt(const string& name, int value);
	void setFloat(const string& name, float value);
private:
	unsigned int m_uiID;		// ��ɫ������ID
};

