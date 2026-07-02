#pragma once
#include<glm/glm.hpp>
class Circle
{
public:
	glm::vec3 pos;
	glm::vec3 vel;
	glm::vec3 scale;
	glm::vec3 colors[2] = {
		glm::vec3(1.0f,1.0f,0.0f),
		glm::vec3(1.0f,0.5f,0.0f),
	};
	glm::vec3 objectColor;
	float radius;
	int textureIndex;
	Circle(glm::vec3, glm::vec3);
	void Move();
	void CheckBounds();
	float GetScaledRadius();
};

