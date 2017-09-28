#pragma once
#include <string>
#include <vector>

#include "glad/glad.h"
#include "CellVec3.h"
#include "Shader.h"

using std::string;
using std::stringstream;
using std::vector;
using cell::Vec2;
using cell::Vec3;

namespace cell
{
	// 约定
	// 材质纹理命名约定：漫反射texture_diffuse[n]; 镜面反射texture_specular[n]
	const unsigned int gMaterialTexTypes = 2;
	const string gStrTexTypeBounds[gMaterialTexTypes] = { "texture_diffuse", "texture_specular" };
	const string gStrMaterialBound = "material";

	template <typename T>
	struct Vertex
	{
		Vec3<T> _Position;	// 位置向量
		Vec3<T> _Normal;		// 法线向量
		Vec2<float> _TexCoords;	// 纹理坐标
	};

	struct Texture
	{
		unsigned int _ID;		// 纹理id
		unsigned int _Type;			// 纹理类型
	};

	template <typename T>
	class Mesh
	{
	public:
		typedef T value_type;
	public:
		vector<Vertex<value_type>> _vertices;	// 位置向量
		vector<unsigned int>	   _indices;	// 索引位置
		vector<Texture>			   _textures;	// 纹理
	public:
		Mesh(const vector<Vertex<value_type>>& vertices, const vector<unsigned int>& indices, const vector<Texture>& textures)
		{
			_vertices = vertices;
			_indices = indices;
			_textures = textures;

			setupMesh();
		}
		~Mesh()
		{
			glDeleteBuffers(1, &m_iVBO);
			glDeleteBuffers(1, &m_iEBO);
			glDeleteVertexArrays(1, &m_iVAO);

			for (size_t i = 0; i < _textures.size(); i++)
			{
				glDeleteTextures(1, &_textures[i]._ID);
			}
		}

		void Draw(CShader* pShader)
		{
			if (pShader == NULL) return;
			unsigned int diffuseNr = 1;
			unsigned int specularNr = 1;

			for (unsigned int i = 0; i < _textures.size(); i++)
			{
				// 激活纹理
				glActiveTexture(GL_TEXTURE0 + i);
				// 组装纹理命名
				stringstream ss;
				string number;
				int type = _textures[i]._Type;	// 获取类型名
				if (type == 0)	// 漫反射材质
				{
					ss << diffuseNr++;
				}
				else if (type == 1) // 镜面反射材质
				{
					ss << specularNr++;
				}
				number = ss.str();

				pShader->setInt(gStrMaterialBound + "." + gStrTexTypeBounds[_textures[i]._Type] + number, i);
				glBindTexture(GL_TEXTURE_2D, _textures[i]._ID);	// 绑定纹理
			}
			
			// 激活默认的纹理
			glActiveTexture(GL_TEXTURE0);
			// 绘制网格
			glBindVertexArray(m_iVAO);
			glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}
	private:
		unsigned int m_iVAO, m_iVBO, m_iEBO;		// opengl对象
		void setupMesh()
		{
			// 生成对象
			glGenVertexArrays(1, &m_iVAO);
			glGenBuffers(1, &m_iVBO);
			glGenBuffers(1, &m_iEBO);

			// 绑定顶点数组对象
			glBindVertexArray(m_iVAO);
			// 绑定顶点缓存对象
			glBindBuffer(GL_ARRAY_BUFFER, m_iVBO);
			glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(Vertex), &_vertices.front(), GL_STATIC_DRAW);
			// 绑定索引对象
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iEBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(_indices)* sizeof(unsigned int), &_indices.front(), GL_STATIC_DRAW);

			// 指定顶点位置
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);	// 位置向量
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, _Normal));	// 法线向量
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, _TexCoords));	// 法线向量
			
			// 解绑对象
			glBindBuffer(GL_ARRAY_BUFFER, m_iVBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iEBO);
			glBindVertexArray(0);
		}
	};

	class Model
	{
	public:
		vector<Texture> _textures_loaded;
	public:
		Model();
		~Model();

	};
}
