#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "glm/glm.hpp"
#include <list>

class Light{

private:

	int id;
	float diffuse[3];
	float position[3];
	float power;

public:
	static int nextlightid;

	void setLightValues(glm::vec3 diff, glm::vec3 pos, float p);

	glm::vec3 getLightDiffuse(){
		return glm::vec3(diffuse[0], diffuse[1], diffuse[2]);
	}
	glm::vec3 getLightPosition(){
		return glm::vec3(position[0], position[1], position[2]);
	}
	float getLightPower(){
		return power;
	}
	int getid(){
		return id;
	}
	void setid(int lightid){
		id = lightid;
	}

	void setLightPosition(glm::vec3 pos){
		position[0] = pos.x;
		position[1] = pos.y;
		position[2] = pos.z;
	}

	void setLightColor(glm::vec3 col){
		diffuse[0] = col.x;
		diffuse[1] = col.y;
		diffuse[2] = col.z;
	}

	void setLightPower(float pow){
		power = pow;
	}
};

#endif