/*================================================================
* Description 模型
* Email huliuworld@yahoo.com
* Created on Thu Jul 04 2019 23:8:36
* Copyright (c) 2019 刘虎
================================================================*/

#include "mesh.hpp"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <string>
#include <vector>

using namespace std;

class Model {
public:
    Model(const char *path);
    void draw(const Shader &shader);

private:
    vector<Mesh> meshes;
    vector<Texture> texturesLoaded;
    string directory;
    void loadModel(string path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName);
};
