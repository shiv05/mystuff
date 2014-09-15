#include "obstaclefactory.hpp"

/*void Obstacle::setAll(Material mat, ObjLoader obj, float e)
{
	m_Material = mat;
	m_ObjLoader = obj;
	restitution = e;
}*/

void ObstacleManager::generateRandomObstacle(glm::vec3 initial_position, BulletWorld& bw, Material& mat)
{
	float mass = 100.0;
	float radius = 0.6;
	
	srand(time(NULL));
	glm::vec3 velocity = glm::vec3(-10.0+rand()%21,-10.0+rand()%21,20+rand()%30);

	float restitution = 2.0;

	addObstacle(mass,radius,initial_position,velocity,restitution,bw,mat);
}

void ObstacleManager::addObstacle(float mass,float radius, glm::vec3 initial_position, glm::vec3 initial_velocity, float restitution,BulletWorld& bw, Material& mat)
{
	btRigidBody* obsbody = bw.addSphere(radius, initial_position.x, 
		initial_position.y, initial_position.z, mass, mat,OBSTACLE);
	obsbody->setRestitution(restitution);
	obsbody->setLinearVelocity(btVector3(initial_velocity.x, initial_velocity.y, initial_velocity.z));
}