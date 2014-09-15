#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "glm/glm.hpp"
#include <list>

class Light{

private:

	int id;
	float diffuse[3];
	float ambient[3];
	float specular[3];
	float shininess;
	float position[4];

public:
	static int nextlightid;

	void Light::setLightValues(glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float shin, glm::vec4 pos)
	{
		diffuse[0] = diff.x;
		diffuse[1] = diff.y;
		diffuse[2] = diff.z;

		ambient[0] = amb.x;
		ambient[1] = amb.y;
		ambient[2] = amb.z;

		specular[0] = spec.x;
		specular[1] = spec.y;
		specular[2] = spec.z;

		shininess = shin;

		position[0] = pos.x;
		position[1] = pos.y;
		position[2] = pos.z;
		position[3] = pos.w;
	}

	glm::vec3 getLightAmbient(){
		return glm::vec3(ambient[0], ambient[1], ambient[2]);
	}
	glm::vec3 getLightDiffuse(){
		return glm::vec3(diffuse[0], diffuse[1], diffuse[2]);
	}
	glm::vec3 getLightSpecular(){
		return glm::vec3(specular[0], specular[1], specular[2]);
	}
	float getLightShininess(){
		return shininess;
	}
	glm::vec4 getLightPosition(){
		return glm::vec4(position[0], position[1], position[2],position[3]);
	}
	int getid(){
		return id;
	}
	void setid(int lightid){
		id = lightid;
	}

	void setLightPosition(glm::vec4 pos){
		position[0] = pos.x;
		position[1] = pos.y;
		position[2] = pos.z;
		position[3] = pos.w;
	}

};

#endif