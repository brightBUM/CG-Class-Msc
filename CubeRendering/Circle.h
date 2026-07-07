#pragma once
#include<glm/glm.hpp>
class Circle
{
public:
	glm::vec3 pos;
	glm::vec3 vel;
	glm::vec3 scale;
	float radius;
	Circle(glm::vec3, glm::vec3);
	void Move(float);
	void CheckBounds();
};

