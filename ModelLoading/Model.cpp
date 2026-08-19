#include "Model.h"

Model::Model(std::string path)
{
	//get the data from assimp

	//go through all the parent / child mesh nodes

	//extract vertices,texture,indices
}

void Model::DrawModel(Shader& shader)
{

	for (int i = 0;i < meshes.size();i++)
	{
		meshes[i].DrawMesh(shader);
	}

}
