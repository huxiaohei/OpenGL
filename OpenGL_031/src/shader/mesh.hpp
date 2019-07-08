/*================================================================
* Description 网格
* Email huliuworld@yahoo.com
* Created on Thu Jul 04 2019 0:13:59
* Copyright (c) 2019 刘虎
================================================================*/

#include "shader.hpp"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <fstream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texcoords;
    glm::vec3 tangent;
    glm::vec3 bitangent;
};

struct Texture {
    unsigned int id;
    string type;
    aiString path;
};

class Mesh {

public:
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;
    GLuint VAO;
    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);
    void draw(const Shader &shader);

private:
    GLuint VBO;
    GLuint EBO;
    void setupMesh();

};
