/*================================================================
* Description Ä£ÐÍ
* Email huxiaoheigame@yahoo.com
* Created on 2019 10 30 22:8:38
* Copyright (c) 2019 huxiaohei
================================================================*/

#include "mesh.hpp"
#include <assimp/postprocess.h>
#include <assimp/scene.h>

using namespace std;

class Model {

	public:
		Model(const char *path, Shader *shader);
		void draw();

	protected:
		vector<Mesh> meshes;
		vector<Texture> texturesLoaded;
		string directory;
		Shader *shader;
		void loadModel(const string &path);
		void processNode(aiNode *node, const aiScene *scene);
		Mesh processMesh(aiMesh *mesh, const aiScene *scene);
		vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName);
};

