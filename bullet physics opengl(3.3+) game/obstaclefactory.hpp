#ifndef OBSTACLEFACTORY_HPP
#define OBSTACLEFACTORY_HPP

#include <ctime>
#include <cstdlib>

#include <list>
#include "material.hpp"
#include "objloader.hpp"
#include "bulletworld.hpp"
#include "helper.hpp"
#include <btBulletDynamicsCommon.h>

/*class Obstacle
{
private:
	Material m_Material;
	ObjLoader m_ObjLoader;
	btRigidBody* body;
	float restitution;


public:
	void setAll(Material mat, ObjLoader obj, float e);

	btRigidBody* getRigidBody(){
		return body;
	}
	float getRestitution(){
		return restitution;
	}
	void setRigidBody(btRigidBody* obsbody){
		body = obsbody;
	}
};*/

namespace ObstacleManager
{
	void generateRandomObstacle(glm::vec3 initial_position, BulletWorld& bw, Material& mat);
	void addObstacle(float mass, float radius, glm::vec3 position, glm::vec3 velocity, float restitution, BulletWorld& bw, Material& mat);
	void checkAndDelete();
};

#endif