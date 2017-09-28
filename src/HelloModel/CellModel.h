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
		Vec3<T> _Position;	// λ������
		Vec3<T> _Normal;		// ��������
		Vec2<float> _TexCoords;	// ��������
	};

	struct Texture
	{
		unsigned int _ID;		// ����id
		string _Type;		// ��ͼ·��
	};

	template <typename T>
	class Mesh
	{
	public:
		typedef T value_type;
	public:
		vector<Vertex<value_type>> _vertices;	// λ������
		vector<unsigned int>	   _indices;	// ����λ��
		vector<Texture>			   _textures;	// ����
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
		unsigned int VAO, VBO, EBO;		// opengl����
		void setupMesh()
		{
			// ���ɶ���
			glGenVertexArrays(1, &VAO);
			glGenBuffers(1, &VBO);
			glGenBuffers(1, &EBO);

			// �󶨶����������
			glBindVertexArray(VAO);
			// �󶨶��㻺�����
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(Vertex), &_vertices.front(), GL_STATIC_DRAW);
			// ����������
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(_indices)* sizeof(unsigned int), &_indices.front(), GL_STATIC_DRAW);

			// ָ������λ��
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);	// λ������
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, _Normal));	// ��������
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, _TexCoords));	// ��������
			
			// ������
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
