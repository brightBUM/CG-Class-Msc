#pragma once
#include<vector>
#include"Mesh.h"
class Model
{
private:
	std::vector<Mesh> meshes;
public:
	Model(std::string path);
	void DrawModel(Shader &shader);
};

