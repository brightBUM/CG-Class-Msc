#include "Mesh.h"

Mesh::Mesh(std::vector<vertex> vertices, std::vector<Texture> textures, std::vector<unsigned int> indices)
{
	this->vertices = vertices;
	this->textures = textures;
	this->indices = indices;

	SetUpMesh();
}

void Mesh::SetUpMesh()
{
	//generate and bind buffers
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertex), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	//vertex attributes - pos,normal,uv
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex,normal));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex,texCoords));

	glBindVertexArray(0);
}

void Mesh::Draw(Shader &shader)
{
	int diffuseNr = 1, specularNr = 1;
	for (int i = 0;i < textures.size();i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		std::string name, num;
		name = textures[i].type;
		if (name == "diffuse")
			num = std::to_string(diffuseNr++);
		else if (name == "specular")
			num = std::to_string(specularNr++);

		shader.SetInt(("material." + name + num).c_str(), i);
		glBindTexture(GL_TEXTURE_2D, textures[i].id);

	}

	glActiveTexture(GL_TEXTURE0);

	//bind vertex
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);


}
