#pragma once
#define STB_IMAGE_IMPLEMENTATION		// ��֤stb_image�����
#include <string>

using std::string;
namespace cell
{
	class CellFileUtil
	{
	public:
		CellFileUtil();
		~CellFileUtil();

		static unsigned int InitTexture(const string& path, const string& directory);	// �����ļ���ʼ���������
	};
}



