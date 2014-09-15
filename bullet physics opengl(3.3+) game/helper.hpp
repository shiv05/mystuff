#ifndef HELPER_HPP
#define HELPER_HPP

#define GLFW_DLL

// Include GLFW
#include <glfw3.h>

#include <glm\glm.hpp>
#include <btBulletDynamicsCommon.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "material.hpp"

glm::mat4 getSphereModelMatrix(btRigidBody* sphere);
glm::mat4 getPlaneModelMatrix(btRigidBody* plane);
glm::mat4 getBoxModelMatrix(btRigidBody* box);


struct RigidBodyTagMaterial{
	btRigidBody* body;
	unsigned short tag;
	Material m_Material;

	/*RigidBodyTagMaterial()
	{
		;
	}
	RigidBodyTagMaterial(const RigidBodyTagMaterial &rbtm)
	{

	}*/
};


void initTaglines(float taglines[100]);

enum RigidBodyTags{
	PLAYER,
	OBSTACLE,
	FLOOR,
	MISC
};

struct KeyState
{
	unsigned int keyCode;
	bool previousState;
	bool currentState;
};

#endif