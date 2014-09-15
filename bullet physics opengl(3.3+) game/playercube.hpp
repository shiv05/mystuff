#ifndef PLAYERCUBE_HPP
#define PLAYERCUBE_HPP

#include "material.hpp"
#include "objloader.hpp"
#include "bulletworld.hpp"
#include "helper.hpp" 

class PlayerCube
{
private:

	float width;
	float height;
	float depth;

	float mass;

	glm::vec3 initial_position;

	Material m_Material;
	ObjLoader m_ObjLoader;
	btRigidBody* body;

public:
	void setAll(Material mat, ObjLoader obj);

	btRigidBody* getRigidBody(){
		return body;
	}
	Material getMaterial(){
		return m_Material;
	}

	glm::vec3 getPosition(){
		return glm::vec3(body->getCenterOfMassPosition().x(), body->getCenterOfMassPosition().y(), 
			body->getCenterOfMassPosition().z());
	}

	void moveCube(int keyCode);

	~PlayerCube(){

	}
};

#endif