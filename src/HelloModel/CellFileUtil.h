#pragma once
//#ifndef STB_IMAGE_IMPLEMENTATION
//#define STB_IMAGE_IMPLEMENTATION		// ��֤stb_image�����
//#endif


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


