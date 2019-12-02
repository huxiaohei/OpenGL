/*================================================================
* Description ����
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
	glm::vec3 position;		// ��������
	glm::vec3 normal;		// ������
	glm::vec2 texcoords;	// ��������
	glm::vec3 tangent;		// ���н�
	glm::vec3 bitangent;	// ���н�
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
