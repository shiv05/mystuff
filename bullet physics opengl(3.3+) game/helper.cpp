#include "helper.hpp"

glm::mat4 getSphereModelMatrix(btRigidBody* sphere)
{
	float r = ((btSphereShape*)sphere->getCollisionShape())->getRadius();
	btTransform t;
	sphere->getMotionState()->getWorldTransform(t);	//get the transform
	float mat[16];
	t.getOpenGLMatrix(mat);	//OpenGL matrix stores the rotation and orientation
	glm::mat4 ret(1.0);
	ret = glm::scale(ret, glm::vec3(r, r, r));
	ret = glm::make_mat4(mat) * ret;
	return ret;
}


glm::mat4 getPlaneModelMatrix(btRigidBody* plane)
{
	btTransform t;
	plane->getMotionState()->getWorldTransform(t);
	float mat[16];
	glm::mat4 ret(1.0);
	t.getOpenGLMatrix(mat);
	//for (int i = 0; i < 16; i++)
	//	printf("%d ",mat[i]);
	//printf("\n");
	ret = glm::make_mat4(mat);
	/*for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			printf("%d ",ret[i][j]);
		}
		printf("\n");
	}*/
	//int temp;
	//scanf("%d",&temp);

	return ret;
}

glm::mat4 getBoxModelMatrix(btRigidBody* box)
{
	btVector3 extent = ((btBoxShape*)box->getCollisionShape())->getHalfExtentsWithoutMargin();
	btTransform t;
	box->getMotionState()->getWorldTransform(t);
	float mat[16];
	t.getOpenGLMatrix(mat);
	glm::mat4 ret(1.0);
	ret = glm::scale(ret, glm::vec3(extent.x(), extent.y(), extent.z()));
	ret = glm::make_mat4(mat) * ret;
	return ret;
}

void initTaglines(float taglines[100])
{
	for (int i = 0; i < 100; i++)
		taglines[i] =  - 100 * (i+1);
}