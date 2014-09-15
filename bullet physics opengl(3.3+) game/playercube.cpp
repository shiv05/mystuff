#include "playercube.hpp"

void PlayerCube::setAll(Material mat, ObjLoader obj)
{
	m_Material = mat;
	m_ObjLoader = obj;

	mass = 10.0;
	
	height = 1.0;
	width = 1.0;
	depth = 1.0;

	initial_position = glm::vec3(0.0,-4.5,0.0);

	//create a rigidbody for the cube
	btTransform t;
	t.setIdentity();
	t.setOrigin(btVector3(initial_position.x,initial_position.y,initial_position.z));
	btBoxShape* box = new btBoxShape(btVector3(width / 2.0, height / 2.0, depth / 2.0));
	btVector3 inertia(0, 0, 0);
	if (mass != 0.0)
		box->calculateLocalInertia(mass, inertia);

	btMotionState* motion = new btDefaultMotionState(t);
	btRigidBody::btRigidBodyConstructionInfo info(mass, motion, box, inertia);
	info.m_angularDamping = 1.0;
	info.m_additionalAngularDampingFactor = 0.9;
	body = new btRigidBody(info);
	body->setFriction(1.0);
	body->setRestitution(0.0);
}

void PlayerCube::moveCube(int keyCode)
{
	if (keyCode == GLFW_KEY_W)
	{
		body->applyForce(btVector3(0.0, 0.0, -100.0), btVector3(0.0, 0.48, -0.4));
		body->applyTorque(btVector3(-650.0, 0.0, 0.0));
	}
	else if (keyCode == GLFW_KEY_S)
	{
		body->applyForce(btVector3(0.0, 0.0, 100.0), btVector3(0.0, 0.48, 0.4));
		body->applyTorque(btVector3(650.0, 0.0, 0.0));
	}
	else if (keyCode == GLFW_KEY_A)
	{
		body->applyForce(btVector3(-100.0, 0.0, 0.0), btVector3(-0.4, 0.48, 0.0));
		body->applyTorque(btVector3(0.0, 0.0, 650.0));
	}
	else if (keyCode == GLFW_KEY_D)
	{
		body->applyForce(btVector3(100.0, 0.0, 0.0), btVector3(0.4, 0.48, 0.0));
		body->applyTorque(btVector3(0.0, 0.0, -650.0));
	}
}