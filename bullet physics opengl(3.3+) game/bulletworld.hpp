#ifndef BULLETWORLD_HPP
#define BULLETWORLD_HPP

#include <btBulletDynamicsCommon.h>
#include <list>
#include "glm/glm.hpp"

#include "objloader.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "helper.hpp"

class BulletWorld
{
private:
	btDiscreteDynamicsWorld* world;
	btDispatcher* dispatcher;
	btCollisionConfiguration* collisionConfig;	//what collision algorithm to use?
	btBroadphaseInterface* broadphase;	//should Bullet examine every object, or just what close to each other
	btConstraintSolver* solver;					//solve collisions, apply forces, impulses

	//vector to store all rigid bodies
	std::list<RigidBodyTagMaterial*> bodies;

public:

	std::list<RigidBodyTagMaterial*> getBodies()
	{
		return bodies;
	}
	btDiscreteDynamicsWorld* getWorld()
	{
		return world;
	}
	void setBodiesList(std::list<RigidBodyTagMaterial*>& bodylist)
	{
		bodies = bodylist;
	}

	void addBody(RigidBodyTagMaterial* rbtm);

	BulletWorld(glm::vec3 gravity);
	~BulletWorld();
	btRigidBody* addFloor(glm::vec3 origin_location, glm::vec3 floor_normal, float plane_constant, Material mat);
	btRigidBody* addSphere(float rad, float x, float y, float z, float mass,Material spheremat,int sphereTag=MISC);
	btRigidBody* addBox(float width, float height, float depth, float x, float y, float z, float mass, Material mat, int boxTag=MISC);

	void addFloor(Material& mat, float z);
	void updateFloor(Material& mat, float z);

	void stepSimulate()
	{
		world->stepSimulation(btScalar(1 / 60.0));
	}

};

#endif