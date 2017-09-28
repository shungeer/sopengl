#pragma once
#include <string>
#include <vector>
#include "glad/glad.h"
#include "CellVec3.h"
#include "Shader.h"

using std::string;
using std::vector;
using cell::Vec2;
using cell::Vec3;

namespace cell
{
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
		string _Type;		// 贴图路径
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
		~Mesh(){}

		void Draw(CShader* shader);
	private:
		unsigned int VAO, VBO, EBO;		// opengl对象
		void setupMesh()
		{
			// 生成对象
			glGenVertexArrays(1, &VAO);
			glGenBuffers(1, &VBO);
			glGenBuffers(1, &EBO);

			// 绑定顶点数组对象
			glBindVertexArray(VAO);
			// 绑定顶点缓存对象
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(Vertex), &_vertices.front(), GL_STATIC_DRAW);
			// 绑定索引对象
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(_indices)* sizeof(unsigned int), &_indices.front(), GL_STATIC_DRAW);

			// 指定顶点位置
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);	// 位置向量
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, _Normal));	// 法线向量
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, _TexCoords));	// 法线向量
			
			// 解绑对象
			glBindVertexArray(0);
		}
	};

	class CellModel
	{
	public:
		CellModel();
		~CellModel();
	};


}
