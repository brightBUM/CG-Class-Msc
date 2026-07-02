#include "Circle.h"
#include"Common.h"
Circle::Circle(glm::vec3 pos,glm::vec3 vel)
{
	this->pos = pos;
	this->vel = vel;
	this->scale = glm::vec3(0.1f);
	this->radius = 0.5f;
	this->objectColor = glm::vec3(0.157f, 0.212f, 0.094f);
	this->textureIndex = Random::RandomInt(0, 4);
	//Log("circle " << this->textureIndex);
}

void Circle::Move()
{
	this->pos += this->vel*(float)deltaTime;
}

void Circle::CheckBounds()
{
	if ((pos.x + (radius * scale.x) > 1.0f) || pos.x - (radius * scale.x) < -1.0f)
	{
		vel.x = -vel.x;
	}
	if ((pos.y + (radius *scale.x) > 1.0f) || (pos.y - (radius * scale.x) < -1.0f))
	{
		vel.y = -vel.y;

	}
}

float Circle::GetScaledRadius()
{
	return radius*scale.x;
}
