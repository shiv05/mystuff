#include "light.hpp"

int Light::nextlightid = 0;

void Light::setLightValues(glm::vec3 diff, glm::vec3 pos, float p)
{
	diffuse[0] = diff.x;
	diffuse[1] = diff.y;
	diffuse[2] = diff.z;

	position[0] = pos.x;
	position[1] = pos.y;
	position[2] = pos.z;

	power = p;
}