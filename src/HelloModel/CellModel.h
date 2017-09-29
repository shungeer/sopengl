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
	// 约定
	// 材质纹理命名约定：漫反射texture_diffuse[n]; 镜面反射texture_specular[n]
	enum MaterialTexType
	{
		DIFFUSE,
		SPECULAR
	};
	const unsigned int gMaterialTexTypes = 2;
	const string gStrTexTypeBounds[gMaterialTexTypes] = { "texture_diffuse", "texture_specular" };		// 和MaterialTexType枚举一一对应
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
		unsigned int _Type;		// 纹理类型
		string		 _TexPath;	// 纹理路径
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
				if (type == MaterialTexType::DIFFUSE)	// 漫反射材质
				{
					ss << diffuseNr++;
				}
				else if (type == MaterialTexType::SPECULAR) // 镜面反射材质
				{
					ss << specularNr++;
				}
				number = ss.str();

				pShader->setInt(gStrMaterialBound + "." + gStrTexTypeBounds[type] + number, i);
				glBindTexture(GL_TEXTURE_2D, _textures[i]._ID);	// 绑定纹理
			}
			// 绘制网格
			glBindVertexArray(VAO());
			glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);

			// 激活默认的纹理
			glActiveTexture(GL_TEXTURE0);
		}

		unsigned int VAO() { return m_iVAO; }
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
			unsigned int size = _vertices.size() * sizeof(Vertex<value_type>);
			glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(Vertex<value_type>), &_vertices.front(), GL_STATIC_DRAW);
			// 绑定索引对象
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iEBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(_indices)* sizeof(unsigned int), &_indices.front(), GL_STATIC_DRAW);

			// 指定顶点位置
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex<value_type>), (void*)0);	// 位置向量
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex<value_type>), (void*)offsetof(Vertex<value_type>, _Normal));	// 法线向量
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex<value_type>), (void*)offsetof(Vertex<value_type>, _TexCoords));	// 法线向量
			
			// 解绑对象
			glBindBuffer(GL_ARRAY_BUFFER, m_iVBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iEBO);
			glBindVertexArray(0);
		}
	};

	template <typename T>
	class Model
	{
	public:
		typedef T value_type;
	public:
		vector<Texture>  _textures_loaded;	// Texture中的纹理路径下标即为此数组的下标
		vector<Mesh<value_type>> _meshs;	// 加载模型文件中所有的mesh
		string _rootDir;		// 模型文件所在根目录
	public:
		Model(const string& path) { this->LoadModel(path); }
		~Model(){}

		void Draw(CShader* pShader)
		{
			for (size_t i = 0; i < this->_meshs.size(); i++)
			{
				this->_meshs[i].Draw(pShader);
			}
		}
	private:
		// 加载模型文件
		void LoadModel(const string& path)
		{
			// 1. 加载模型文件
			Assimp::Importer importer;
			// 处理选项
			// aiProcess_Triangulate:如果模型不是（全部）由三角形组成，它需要将模型所有的图元形状变换为三角形
			// aiProcess_FlipUVs将在处理的时候翻转y轴的纹理坐标
			const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

			// 判断是否加载成功
			if (!scene || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) || !scene->mRootNode)
			{
				cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
				return;
			}

			// 2.获取文件路径
			int index = path.find_last_of('/');
			if (index == -1)
			{
				index = path.find_last_of('\\');
			}
			this->_rootDir = path.substr(0, index);

			// 3.处理节点获取mesh
			this->ProcessNode(scene->mRootNode, scene);
		}

		// 处理assimp的节点
		void ProcessNode(const aiNode* node, const aiScene* scene)
		{
			// 节点中的mesh和material仅仅保存索引，实际数据存储在scene中
			for (unsigned int i = 0; i < node->mNumMeshes; i++)
			{
				this->_meshs.push_back(this->BuildMesh(scene->mMeshes[node->mMeshes[i]], scene));
			}

			// 处理这个节点的多有子节点
			for (unsigned int i = 0; i < node->mNumChildren; i++)
			{
				this->ProcessNode(node->mChildren[i], scene);
			}
		}

		// 构建mesh
		Mesh<value_type> BuildMesh(const aiMesh* mesh, const aiScene* scene)
		{
			vector<Vertex<value_type>> vertices;
			vector<unsigned int> indices;
			vector<Texture> textures;

			// 遍历每个mesh的顶点
			for (unsigned int i = 0; i < mesh->mNumVertices; i++)
			{
				Vertex<value_type> vertex;
				Vec3<value_type> tmpV;
				// 位置
				vertex._Position = Vec3<value_type>(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
				// 法向量
				vertex._Normal = Vec3<value_type>(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
				// 纹理坐标
				if (mesh->mTextureCoords[0])		// 判断纹理坐标是否存在	如果模型有多种纹理坐标仅仅取第一个
				{
					vertex._TexCoords = Vec2<float>(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
				}
				else // 不存在的话，默认为0
				{
					vertex._TexCoords = Vec2<float>(0.f, 0.f);
				}
				vertices.push_back(vertex);
			}

			// 遍历所有面，抽取索引
			for (unsigned int i = 0; i < mesh->mNumFaces; i++)
			{
				aiFace face = mesh->mFaces[i];
				for (unsigned int j = 0; j < face.mNumIndices; j++) indices.push_back(face.mIndices[j]);
			}

			// 抽取材质属性
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			// 漫反射贴图
			this->LoadMaterialTextures(textures, material, aiTextureType_DIFFUSE, MaterialTexType::DIFFUSE);
			// 镜面反射贴图
			this->LoadMaterialTextures(textures, material, aiTextureType_SPECULAR, MaterialTexType::SPECULAR);

			return Mesh<value_type>(vertices, indices, textures);
		}

		// 加载材质
		void LoadMaterialTextures(vector<Texture>& textures, const aiMaterial* mat, aiTextureType type, MaterialTexType mType)
		{
			vector<Texture>::const_iterator citer;
			unsigned int count = mat->GetTextureCount(type);			// 节点材质数量
			for (size_t i = 0; i < count; i++)
			{
				aiString str;
				mat->GetTexture(type, i, &str);		// 得到纹理路径
				bool skip = false;
				for (citer = _textures_loaded.cbegin(); citer != _textures_loaded.cend(); citer++)
				{
					if (strcmp((*citer)._TexPath.c_str(), str.C_Str()) == 0)
					{
						skip = true;
						break;
					}
				}

				Texture texture;
				if (!skip)	// 纹理已经被加载，不需要重复加载
				{
					texture._ID = cell::CellFileUtil::InitTexture(str.C_Str(), this->_rootDir);
					texture._Type = mType;
					texture._TexPath = str.C_Str();
					this->_textures_loaded.push_back(texture);			// 添加纹理
				}
				else
				{
					texture = (*citer);
				}
				
				textures.push_back(texture);
			}
		}
	};
}
