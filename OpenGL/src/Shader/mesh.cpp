/*================================================================
* Description Íø¸ñ
* Email huxiaoheigame@yahoo.com
* Created on 2019 10 30 20:44:23
* Copyright (c) 2019 huxiaohei
================================================================*/

#include "shader/mesh.hpp"

Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures, Shader *shader):
	vertices(vertices), indices(indices), textures(textures), shader(shader){
	setupMesh();
}

void Mesh::setupMesh() {

	glGenVertexArrays(1, &vertexArrayObject);
	glGenBuffers(1, &vertexBufferObject);
	glGenBuffers(1, &elementBufferObject);

	glBindVertexArray(vertexArrayObject);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObject);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
	
	shader->setVertexAttributePointer("position", 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
	shader->setVertexAttributePointer("normal", 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, normal));
	shader->setVertexAttributePointer("texcoords", 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, texcoords));
	shader->setVertexAttributePointer("tangent", 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, tangent));
	shader->setVertexAttributePointer("bitangent", 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, bitangent));

	glBindVertexArray(0);
}

void Mesh::draw() {
	
	unsigned int diffuserNr = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr = 1;
	unsigned int heightNr = 1;
	for (unsigned int i = 0; i < textures.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		string number;
		string type = textures[i].type;
		if (type == "texture_diffuse") {
			number = to_string(diffuserNr++);
		}
		else if (type == "texture_specular") {
			number = to_string(specularNr++);
		}
		else if (type == "texture_normal") {
			number = to_string(normalNr++);
		}
		else if (type == "texture_height") {
			number = to_string(heightNr++);
		}
		shader->setUniformIntByName((type + number), i);
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}
	glBindVertexArray(vertexArrayObject);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glActiveTexture(GL_TEXTURE0);
}


Mesh::~Mesh() {
	
}

