#pragma once
#include <string>
#include <sstream>
#include <vector>

#include "glad/glad.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "CellVec3.h"
#include "CellFileUtil.h"
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
	enum MaterialTexType
	{
		DIFFUSE,
		SPECULAR
	};
	const unsigned int gMaterialTexTypes = 2;
	const string gStrTexTypeBounds[gMaterialTexTypes] = { "texture_diffuse", "texture_specular" };		// ��MaterialTexTypeö��һһ��Ӧ
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
		unsigned int _Type;		// ��������
		string		 _TexPath;	// ����·��
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
		~Mesh() {}

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
				if (type == MaterialTexType::DIFFUSE)	// ���������
				{
					ss << diffuseNr++;
				}
				else if (type == MaterialTexType::SPECULAR) // ���淴�����
				{
					ss << specularNr++;
				}
				number = ss.str();

				pShader->setInt(gStrMaterialBound + "." + gStrTexTypeBounds[type] + number, i);
				glBindTexture(GL_TEXTURE_2D, _textures[i]._ID);	// ������
			}
			// ��������
			glBindVertexArray(m_iVAO);
			glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);

			// ����Ĭ�ϵ�����
			glActiveTexture(GL_TEXTURE0);
		}
		void ClearMesh()
		{
			glDeleteBuffers(1, &m_iVBO);
			glDeleteBuffers(1, &m_iEBO);
			glDeleteVertexArrays(1, &m_iVAO);

			for (size_t i = 0; i < _textures.size(); i++)
			{
				glDeleteTextures(1, &_textures[i]._ID);
			}
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
			glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(Vertex<value_type>), &_vertices.front(), GL_STATIC_DRAW);
			// ����������
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iEBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(_indices)* sizeof(unsigned int), &_indices.front(), GL_STATIC_DRAW);

			// ָ������λ��
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex<value_type>), (void*)0);	// λ������
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex<value_type>), (void*)offsetof(Vertex<value_type>, _Normal));	// ��������
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex<value_type>), (void*)offsetof(Vertex<value_type>, _TexCoords));	// ��������
			
			// ������
			glBindVertexArray(0);	// �Ƚ��vao����Ϊvbo ebo��Ҫ�ڻָ�vaoʱ��ʹ��
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}
	};

	template <typename T>
	class Model
	{
	public:
		typedef T value_type;
	public:
		vector<Texture>  _textures_loaded;	// Texture�е�����·���±꼴Ϊ��������±�
		vector<Mesh<value_type>> _meshs;	// ����ģ���ļ������е�mesh
		string _rootDir;		// ģ���ļ����ڸ�Ŀ¼
	public:
		Model(const string& path) { this->LoadModel(path); }
		~Model()
		{
			vector<Mesh<value_type>>::iterator itr;
			for (itr = _meshs.begin(); itr != _meshs.end(); itr++)
			{
				(*itr).ClearMesh();
			}
		}

		void Draw(CShader* pShader)
		{
			for (size_t i = 0; i < this->_meshs.size(); i++)
			{
				this->_meshs[i].Draw(pShader);
			}
		}
	private:
		// ����ģ���ļ�
		void LoadModel(const string& path)
		{
			// 1. ����ģ���ļ�
			Assimp::Importer importer;
			// ����ѡ��
			// aiProcess_Triangulate:���ģ�Ͳ��ǣ�ȫ��������������ɣ�����Ҫ��ģ�����е�ͼԪ��״�任Ϊ������
			// aiProcess_FlipUVs���ڴ����ʱ��תy�����������
			const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

			// �ж��Ƿ���سɹ�
			if (!scene || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) || !scene->mRootNode)
			{
				cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
				return;
			}

			// 2.��ȡ�ļ�·��
			int index = path.find_last_of('/');
			if (index == -1)
			{
				index = path.find_last_of('\\');
			}
			this->_rootDir = path.substr(0, index);

			// 3.����ڵ��ȡmesh
			this->ProcessNode(scene->mRootNode, scene);
		}

		// ����assimp�Ľڵ�
		void ProcessNode(const aiNode* node, const aiScene* scene)
		{
			// �ڵ��е�mesh��material��������������ʵ�����ݴ洢��scene��
			for (unsigned int i = 0; i < node->mNumMeshes; i++)
			{
				this->_meshs.push_back(this->BuildMesh(scene->mMeshes[node->mMeshes[i]], scene));
			}

			// ��������ڵ�Ķ����ӽڵ�
			for (unsigned int i = 0; i < node->mNumChildren; i++)
			{
				this->ProcessNode(node->mChildren[i], scene);
			}
		}

		// ����mesh
		Mesh<value_type> BuildMesh(const aiMesh* mesh, const aiScene* scene)
		{
			vector<Vertex<value_type>> vertices;
			vector<unsigned int> indices;
			vector<Texture> textures;

			// ����ÿ��mesh�Ķ���
			for (unsigned int i = 0; i < mesh->mNumVertices; i++)
			{
				Vertex<value_type> vertex;
				// λ��
				vertex._Position = Vec3<value_type>(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
				// ������
				vertex._Normal = Vec3<value_type>(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
				// ��������
				if (mesh->mTextureCoords[0])		// �ж����������Ƿ����	���ģ���ж��������������ȡ��һ��
				{
					vertex._TexCoords = Vec2<float>(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
				}
				else // �����ڵĻ���Ĭ��Ϊ0
				{
					vertex._TexCoords = Vec2<float>(0.f, 0.f);
				}
				vertices.push_back(vertex);
			}

			// ���������棬��ȡ����
			for (unsigned int i = 0; i < mesh->mNumFaces; i++)
			{
				aiFace face = mesh->mFaces[i];
				for (unsigned int j = 0; j < face.mNumIndices; j++) indices.push_back(face.mIndices[j]);
			}

			if (mesh->mMaterialIndex >= 0)
			{
				// ��ȡ��������
				aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
				// ��������ͼ
				this->LoadMaterialTextures(textures, material, aiTextureType_DIFFUSE, MaterialTexType::DIFFUSE);
				// ���淴����ͼ
				this->LoadMaterialTextures(textures, material, aiTextureType_SPECULAR, MaterialTexType::SPECULAR);
			}
			
			return Mesh<value_type>(vertices, indices, textures);
		}

		// ���ز���
		void LoadMaterialTextures(vector<Texture>& textures, const aiMaterial* mat, aiTextureType type, MaterialTexType mType)
		{
			vector<Texture>::const_iterator citer;
			unsigned int count = mat->GetTextureCount(type);			// �ڵ��������
			for (size_t i = 0; i < count; i++)
			{
				aiString str;
				mat->GetTexture(type, i, &str);		// �õ�����·��
				bool skip = false;
				for (citer = _textures_loaded.cbegin(); citer != _textures_loaded.cend(); citer++)
				{
					if (strcmp((*citer)._TexPath.c_str(), str.C_Str()) == 0)
					{
						textures.push_back(*citer);
						skip = true;
						break;
					}
				}
				if (!skip)	// �����Ѿ������أ�����Ҫ�ظ�����
				{
					Texture texture;
					texture._ID = cell::CellFileUtil::InitTexture(str.C_Str(), this->_rootDir);
					texture._Type = mType;
					texture._TexPath = str.C_Str();
					this->_textures_loaded.push_back(texture);			// �������
					textures.push_back(texture);
				}
			}
		}
	};
}
