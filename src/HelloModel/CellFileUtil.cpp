#include "CellFileUtil.h"

#include <iostream>

#include "glad/glad.h"
#include "stb_image.h"

namespace cell
{
	unsigned int CellFileUtil::InitTexture(const string& path, const string& directory)
	{
		string filename = directory + '/' + path;
		
		unsigned int texID;
		glGenTextures(1, &texID);	// �����������
		
		// stb_image����ͼƬ
		int width, height, nrComponents;
		unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
		if (data)
		{
			GLenum format;
			if (nrComponents == 1) format = GL_RED;
			else if (nrComponents == 3) format = GL_RGB;
			else if (nrComponents == 4) format = GL_RGBA;

			// ������
			glBindTexture(GL_TEXTURE_2D, texID);
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);	// ����mipmap

			// ָ���������
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
		else
		{
			std::cout << "Texture failed to load at path: " << path << std::endl;
		}

		// �ͷ�����
		stbi_image_free(data);

		return texID;
	}
}