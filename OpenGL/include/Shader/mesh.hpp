/*================================================================
* Description 网格
* Email huxiaoheigame@yahoo.com
* Created on 2019 10 30 20:44:5
* Copyright (c) 2019 huxiaohei
================================================================*/

#include "shader.hpp"
#include <assimp/Importer.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct Vertex {
	glm::vec3 position;		// 顶点数据
	glm::vec3 normal;		// 法向量
	glm::vec2 texcoords;	// 纹理坐标
	glm::vec3 tangent;		// 内切角
	glm::vec3 bitangent;	// 外切角
};


struct Texture {
	unsigned int id;
	string type;
	aiString path;
};

class Mesh {
	public:
		Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures, Shader *shader);
		~Mesh();
		void setupMesh();
		void draw();
	protected:
		vector<Vertex> vertices;
		vector<unsigned int> indices;
		vector<Texture> textures;
		Shader *shader;
		GLuint vertexArrayObject;
		GLuint vertexBufferObject;
		GLuint elementBufferObject;
};
