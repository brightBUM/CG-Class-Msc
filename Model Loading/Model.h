#include<string>
#include"shader.h"
#include"Mesh.h"
#include<vector>
#include<stb_image.h>
#include<assimp/Importer.hpp>
#include<assimp/scene.h>
#include<assimp/postprocess.h>
#pragma once

unsigned int LoadTexture(const char* path, const std::string& directory);

class Model
{
private:
	std::string directory;
	std::vector<Mesh> meshes;
	std::vector<Texture> loadedTextures;
	std::vector<Texture> LoadMaterialTextures(aiMaterial* mat,
		aiTextureType type, std::string typeName);
	void LoadModel(std::string path);
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
public:
	Model(const char* path);
	void Draw(Shader& shader);

};

