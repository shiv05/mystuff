#include "bulletworld.hpp"

BulletWorld::BulletWorld(glm::vec3 gravity)
{
	collisionConfig = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfig);
	broadphase = new btDbvtBroadphase();
	solver = new btSequentialImpulseConstraintSolver();
	world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfig);
	world->setGravity(btVector3(gravity.x, gravity.y, gravity.z));
	world->getSolverInfo().m_splitImpulse = true;
}

BulletWorld::~BulletWorld()
{
	for (std::list<RigidBodyTagMaterial*>::iterator it = bodies.begin(); it != bodies.end(); ++it)
	{
		world->removeCollisionObject((*it)->body);
		btMotionState* motionState = ((*it)->body)->getMotionState();
		btCollisionShape* shape = ((*it)->body)->getCollisionShape();
		delete ((*it)->body);
		delete shape;
		delete motionState;
	}
	delete dispatcher;
	delete collisionConfig;
	delete solver;
	delete broadphase;
	delete world;
}

void BulletWorld::addBody(RigidBodyTagMaterial* rbtm)
{
	world->addRigidBody(rbtm->body);
	bodies.push_back(rbtm);
}

btRigidBody* BulletWorld::addSphere(float rad, float x, float y, float z, float mass, Material spheremat, int spheretag)
{
	btTransform t;	//position and rotation
	t.setIdentity();
	t.setOrigin(btVector3(x, y, z));	//put it to x,y,z coordinates
	btSphereShape* sphere = new btSphereShape(rad);
	btVector3 inertia(0, 0, 0);	//inertia is 0,0,0 for static object, else
	if (mass != 0.0)
		sphere->calculateLocalInertia(mass, inertia);	//it can be determined by this function (for all kind of shapes)

	btMotionState* motion = new btDefaultMotionState(t);	//set the position (and motion)
	btRigidBody::btRigidBodyConstructionInfo info(mass, motion, sphere, inertia);	//create the constructioninfo, you can create multiple bodies with the same info
	btRigidBody* body = new btRigidBody(info);	//let's create the body itself
	world->addRigidBody(body);	//and let the world know about it
	RigidBodyTagMaterial* temp = new RigidBodyTagMaterial;
	temp->body = body;
	temp->tag = spheretag;
	temp->m_Material = spheremat;
	bodies.push_back(temp);
	return body;
}

btRigidBody* BulletWorld::addFloor(glm::vec3 origin_location, glm::vec3 floor_normal, float planeconstant, Material mat)
{
	btTransform t;
	t.setIdentity();
	t.setOrigin(btVector3(origin_location.x, origin_location.y, origin_location.z));
	//btStaticPlaneShape* plane = new btStaticPlaneShape(btVector3(0.0, 1.0, 0.0), (btScalar)2.5);
	btStaticPlaneShape* plane = new btStaticPlaneShape(btVector3(floor_normal.x, floor_normal.y, floor_normal.z), (btScalar)planeconstant);
	btMotionState* motion = new btDefaultMotionState(t);
	btRigidBody::btRigidBodyConstructionInfo info(0.0, motion, plane);
	btRigidBody* body = new btRigidBody(info);
	world->addRigidBody(body);
	RigidBodyTagMaterial* temp = new RigidBodyTagMaterial;
	temp->body = body;
	temp->tag = MISC;
	temp->m_Material = mat;
	bodies.push_back(temp);
	return body;
}

btRigidBody* BulletWorld::addBox(float width, float height, float depth, float x, float y, float z, float mass,Material mat, int boxTag)
{
	btTransform t;
	t.setIdentity();
	t.setOrigin(btVector3(x, y, z));
	btBoxShape* box = new btBoxShape(btVector3(width / 2.0, height / 2.0, depth / 2.0));
	btVector3 inertia(0, 0, 0);
	if (mass != 0.0)
		box->calculateLocalInertia(mass, inertia);

	btMotionState* motion = new btDefaultMotionState(t);
	btRigidBody::btRigidBodyConstructionInfo info(mass, motion, box, inertia);
	btRigidBody* body = new btRigidBody(info);
	body->setRestitution(0.5);
	RigidBodyTagMaterial* temp = new RigidBodyTagMaterial;
	world->addRigidBody(body);
	temp->body = body;
	temp->tag = boxTag;
	if (boxTag == FLOOR)
	{
		body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
		body->setActivationState(DISABLE_DEACTIVATION);
	}
	temp->m_Material = mat;
	bodies.push_back(temp);
	return body;
}


void BulletWorld::addFloor(Material& mat, float z)
{
	for (int i = -50; i < 300; i += 10)
	{
		//m_BulletWorld.addBox(10.0, 1.0, 10000.0, 0.0, -5.0, 0.0, 0.0, JADE);
		addBox(10.0, 1.0, 10.0, 0.0, -5.0, -i, 0.0, mat, FLOOR);
		addBox(10.0, 1.0, 10.0, 0.0, 5.0, -i, 0.0, mat, FLOOR);
		addBox(1.0, 10.0, 10.0, -5.0, 0.0, -i, 0.0, mat, FLOOR);
		addBox(1.0, 10.0, 10.0, 5.0, 0.0, -i, 0.0, mat, FLOOR);
	}
}

void BulletWorld::updateFloor(Material& mat, float z)
{
	std::list<RigidBodyTagMaterial*> bodies = getBodies();
	std::list<RigidBodyTagMaterial*> newbodies;

	for (std::list<RigidBodyTagMaterial*>::iterator it = bodies.begin(); it != bodies.end();)
	{
		if ((*it)->tag != FLOOR)
		{
			newbodies.push_back((*it));
			it++;
		}
		else
		{
			RigidBodyTagMaterial* temp = (*it);
			it = bodies.erase(it);
			world->removeRigidBody(temp->body);
			delete temp->body->getMotionState();
			delete temp->body->getCollisionShape();
			delete temp->body;
			delete temp;
		}
	}
	bodies.clear();
	setBodiesList(newbodies);

	//printf("size:%d rigidsize:%d colshapes:%d \n",bodies.size(),world->getNumCollisionObjects(),);

	for (int i = -50; i < 300; i += 10)
	{
		//m_BulletWorld.addBox(10.0, 1.0, 10000.0, 0.0, -5.0, 0.0, 0.0, JADE);
		addBox(10.0, 1.0, 10.0, 0.0, -5.0, z - i, 0.0, mat, FLOOR);
		addBox(10.0, 1.0, 10.0, 0.0, 5.0, z - i, 0.0, mat, FLOOR);
		addBox(1.0, 10.0, 10.0, -5.0, 0.0, z - i, 0.0, mat, FLOOR);
		addBox(1.0, 10.0, 10.0, 5.0, 0.0, z - i, 0.0, mat, FLOOR);
	}
}



///TODO: find somebody who can tell why the hell this is not working
/*void updateFloor(BulletWorld& bw, std::list<Light> lights, Material& mat, float z)
{
std::list<RigidBodyTagMaterial*> bodies = bw.getBodies();
btDiscreteDynamicsWorld* world = bw.getWorld();

printf("%f\n", z);

for (std::list<RigidBodyTagMaterial*>::iterator it = bodies.begin(); it != bodies.end();)
{
if ((*it)->tag == FLOOR)
{
//btTransform t = (*it)->body->getCenterOfMassTransform();
//btVector3 newOrigin = t.getOrigin();
//newOrigin.setZ(newOrigin.z()-100);
//t.setOrigin(newOrigin);
//(*it)->body->setCenterOfMassTransform(t);
RigidBodyTagMaterial* temp = (*it);
it = bodies.erase(it);
world->removeRigidBody(temp->body);
delete temp->body->getMotionState();
delete temp->body;
//delete temp->body->getCollisionShape();
delete temp;
}
else
{
it++;
}
}

for (std::list<Light>::iterator it = lights.begin(); it != lights.end(); it++)
{
glm::vec3 lightPos = (*it).getLightPosition();
(*it).setLightPosition(glm::vec3(lightPos.x,lightPos.y,lightPos.z-100));
}

//printf("size:%d rigidsize:%d colshapes:%d \n",bodies.size(),world->getNumCollisionObjects(),);

for (int i = -50; i < 200; i+=10)
{
//m_BulletWorld.addBox(10.0, 1.0, 10000.0, 0.0, -5.0, 0.0, 0.0, JADE);
bw.addBox(10.0,1.0,10.0, 0.0,-5.0,z-i,0.0,mat,FLOOR);
bw.addBox(10.0, 1.0, 10.0, 0.0, 5.0, z-i, 0.0, mat,FLOOR);
bw.addBox(1.0, 10.0, 10.0, -5.0, 0.0, z-i, 0.0, mat,FLOOR);
bw.addBox(1.0, 10.0, 10.0, 5.0, 0.0, z-i, 0.0, mat,FLOOR);
}
}*/