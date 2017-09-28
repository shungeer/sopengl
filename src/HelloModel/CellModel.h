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
	// Լ��
	// ������������Լ����������texture_diffuse[n]; ���淴��texture_specular[n]
	const unsigned int gMaterialTexTypes = 2;
	const string gStrTexTypeBounds[gMaterialTexTypes] = { "texture_diffuse", "texture_specular" };
	const string gStrMaterialBound = "material";

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
		unsigned int _Type;			// ��������
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
				// ��������
				glActiveTexture(GL_TEXTURE0 + i);
				// ��װ��������
				stringstream ss;
				string number;
				int type = _textures[i]._Type;	// ��ȡ������
				if (type == 0)	// ���������
				{
					ss << diffuseNr++;
				}
				else if (type == 1) // ���淴�����
				{
					ss << specularNr++;
				}
				number = ss.str();

				pShader->setInt(gStrMaterialBound + "." + gStrTexTypeBounds[_textures[i]._Type] + number, i);
				glBindTexture(GL_TEXTURE_2D, _textures[i]._ID);	// ������
			}
			
			// ����Ĭ�ϵ�����
			glActiveTexture(GL_TEXTURE0);
			// ��������
			glBindVertexArray(m_iVAO);
			glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}
	private:
		unsigned int m_iVAO, m_iVBO, m_iEBO;		// opengl����
		void setupMesh()
		{
			// ���ɶ���
			glGenVertexArrays(1, &m_iVAO);
			glGenBuffers(1, &m_iVBO);
			glGenBuffers(1, &m_iEBO);

			// �󶨶����������
			glBindVertexArray(m_iVAO);
			// �󶨶��㻺�����
			glBindBuffer(GL_ARRAY_BUFFER, m_iVBO);
			glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(Vertex), &_vertices.front(), GL_STATIC_DRAW);
			// ����������
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iEBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(_indices)* sizeof(unsigned int), &_indices.front(), GL_STATIC_DRAW);

			// ָ������λ��
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);	// λ������
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, _Normal));	// ��������
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, _TexCoords));	// ��������
			
			// ������
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
