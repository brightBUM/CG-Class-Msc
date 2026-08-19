#pragma once
#include<glm/glm.hpp>
#include<vector>
#include<string>
#include"Shader.h"
struct Vertex
{
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec3 uv;
};
struct Texture
{
	unsigned int id;
	std::string path;
};
class Mesh
{
private:
	std::vector<Vertex> vertices;
	std::vector<Texture> textures;
	std::vector<unsigned int> indices;
	unsigned int VBO, VAO, EBO;
public:
	Mesh(std::vector<Vertex> vertices, std::vector<Texture> textures, std::vector<unsigned int> indices);
	void SetupMesh();
	void DrawMesh(Shader &shader);
};

