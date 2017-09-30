#pragma once
//#ifndef STB_IMAGE_IMPLEMENTATION
//#define STB_IMAGE_IMPLEMENTATION		// 保证stb_image库加载
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

		static unsigned int InitTexture(const string& path, const string& directory);	// 加载文件初始化纹理对象
	};
}


