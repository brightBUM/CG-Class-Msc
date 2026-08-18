#pragma once
#include<glm/glm.hpp>
#include<string>
#include<vector>
#include"Shader.h"
struct vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
};

struct Texture
{
	unsigned int id;
	std::string type;
	std::string path;
};


class Mesh
{
		std::vector<vertex> vertices;
		std::vector<Texture> textures;
		std::vector<unsigned int> indices;

		unsigned int VAO, VBO, EBO;

	public :
		Mesh(std::vector<vertex> vertices, std::vector<Texture> textures,
			std::vector<unsigned int> indices);
		void SetUpMesh();
		void Draw(Shader&);
};

