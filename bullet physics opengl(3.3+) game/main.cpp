#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <list>

// Include GLEW
#include <GL/glew.h>

#define GLFW_DLL

// Include GLFW
#include <glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;

#include "camera.hpp"
#include "shader.hpp"
#include "objloader.hpp"
#include "bulletworld.hpp"
#include "helper.hpp"
#include "material.hpp"
#include "playercube.hpp"
#include "keyboard.hpp"
#include "tpcamera.hpp"
#include "light.hpp"
#include "lightmanager.hpp"
#include "obstaclefactory.hpp"


int main(void)
{

	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(1024, 768, "Shadows", NULL, NULL);
	if (window == NULL){
		fprintf(stderr, "Failed to open GLFW window\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	glfwSetCursorPos(window, 1024 / 2, 768 / 2);

	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);

	GLuint programID = LoadShaders("vertex.vert", "fragment.frag");

	GLuint VID = glGetUniformLocation(programID, "V");
	GLuint PID = glGetUniformLocation(programID, "P");
	GLuint MID = glGetUniformLocation(programID, "M");
	GLuint NMID = glGetUniformLocation(programID, "NormalMatrix");
	GLuint mat_ambID = glGetUniformLocation(programID, "mat_amb");
	GLuint mat_diffID = glGetUniformLocation(programID, "mat_diff");
	GLuint mat_specID = glGetUniformLocation(programID, "mat_spec");
	GLuint mat_shinID = glGetUniformLocation(programID, "mat_shin");
	GLuint num_lights_usedID = glGetUniformLocation(programID, "num_lights_used");
	GLuint light_position_wsID = glGetUniformLocation(programID, "lightpos_ws");
	GLuint light_colorID = glGetUniformLocation(programID, "lightcolor");
	GLuint lightpowerID = glGetUniformLocation(programID, "lightpower");




	TPCamera m_TPCamera;
	ObjLoader suzanne;
	suzanne.initObj("suzanne.obj", true);
	ObjLoader sphere;
	sphere.initObj("sphere.obj", false);
	ObjLoader plane;
	plane.initObj("plane.obj", false);
	ObjLoader cube;
	cube.initObj("cube.obj", false);

	Material BRONZE;
	BRONZE.setMaterialValues(glm::vec3(0.2125, 0.1275, 0.054), glm::vec3(0.714, 0.4284, 0.18144), glm::vec3(0.393548, 0.271906, 0.166721), 0.2);
	Material JADE;
	JADE.setMaterialValues(glm::vec3(0.635, 0.7225, 0.6575), glm::vec3(0.94, 0.99, 0.93), glm::vec3(0.316228, 0.316228, 0.316228), 0.1);
	Material SILVER;
	SILVER.setMaterialValues(glm::vec3(0.19225, 0.19225, 0.19225), glm::vec3(0.50754, 0.50754, 0.50754), glm::vec3(0.508273, 0.508273, 0.508273), 0.4);


	Light::nextlightid = 0;
	LightManager m_LightManager;

	Light* LIGHT1 = new Light;
	LIGHT1->setLightValues(glm::vec3(1.0, 1.0, 1.0), glm::vec3(0.0, 0.0, 0.0), 5.0);

	Light* LIGHT2 = new Light;
	LIGHT2->setLightValues(glm::vec3(1.0, 1.0, 1.0), glm::vec3(0.0, 1.0, -100.0), 10.0);

	Light* LIGHT3 = new Light;
	LIGHT3->setLightValues(glm::vec3(1.0, 1.0, 1.0), glm::vec3(0.0, 1.0, -50.0), 10.0);

	Light* LIGHT4 = new Light;
	LIGHT4->setLightValues(glm::vec3(1.0, 1.0, 1.0), glm::vec3(0.0, 1.0, -0.0), 10.0);

	Light* LIGHT5 = new Light;
	LIGHT5->setLightValues(glm::vec3(1.0, 1.0, 1.0), glm::vec3(0.0, 1.0, -150.0), 10.0);

	m_LightManager.addLight(LIGHT1);
	m_LightManager.addLight(LIGHT2);
	m_LightManager.addLight(LIGHT3);
	m_LightManager.addLight(LIGHT4);
	m_LightManager.addLight(LIGHT5);

	BulletWorld m_BulletWorld(glm::vec3(0.0, -10.0, 0.0));

	PlayerCube m_PlayerCube;
	m_PlayerCube.setAll(BRONZE, cube);

	RigidBodyTagMaterial* temp = new RigidBodyTagMaterial;
	temp->body = m_PlayerCube.getRigidBody();
	temp->tag = PLAYER;
	temp->m_Material = m_PlayerCube.getMaterial();

	m_BulletWorld.addBody(temp);

	m_BulletWorld.addFloor(JADE, m_PlayerCube.getPosition().z);

	m_BulletWorld.addBox(10.0, 1.0, 10000.0, 0.0, -5.0, 0.0, 0.0,JADE);
	m_BulletWorld.addBox(10.0, 1.0, 10000.0, 0.0, 5.0, 0.0, 0.0,JADE);
	m_BulletWorld.addBox(1.0, 10.0, 10000.0, -5.0, 0.0, 0.0, 0.0,JADE);
	m_BulletWorld.addBox(1.0, 10.0, 10000.0, 5.0, 0.0, 0.0, 0.0,JADE);


	Keyboard keyboard;
	keyboard.initKeyboard();

	int framecounter = 0;
	int tagIndex = 0;
	float taglines[100];
	initTaglines(taglines);
	do{
		glm::vec3 playerPos = m_PlayerCube.getPosition();
		std::list<Light*> lights = m_LightManager.getLights();

		if (playerPos.z < taglines[tagIndex])
		{
			m_BulletWorld.updateFloor(JADE, taglines[tagIndex]);
			for (std::list<Light*>::iterator it = lights.begin(); it != lights.end(); it++)
			{
				glm::vec3 lightPos = (*it)->getLightPosition();
				(*it)->setLightPosition(glm::vec3(lightPos.x, lightPos.y, lightPos.z - 100.0));
			}
			tagIndex++;
		}


		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, 1024, 768);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(programID);

		m_TPCamera.computeMatricesFromInputs(playerPos);
		glm::mat4 ProjectionMatrix = m_TPCamera.getProjectionMatrix();
		glm::mat4 ViewMatrix = m_TPCamera.getViewMatrix();
		glm::mat4 ModelMatrix = glm::mat4(1.0);
		glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glm::mat4 NormalMatrix = glm::transpose(glm::inverse(ViewMatrix * ModelMatrix));

		glUniformMatrix4fv(VID, 1, GL_FALSE, &ViewMatrix[0][0]);
		glUniformMatrix4fv(PID, 1, GL_FALSE, &ProjectionMatrix[0][0]);
		glUniformMatrix4fv(NMID, 1, GL_FALSE, &NormalMatrix[0][0]);
		glUniformMatrix4fv(MID, 1, GL_FALSE, &ModelMatrix[0][0]);

		int numlights = m_LightManager.getLights().size();
		glUniform1i(num_lights_usedID, numlights);

		//read all lights into an array
		float lightpos[30];
		float lightcolor[30];
		float lightpow[30];

		std::list<Light*>::iterator it = lights.begin();
		(*it)->setLightPosition(glm::vec3(0.0, 0.0, (m_PlayerCube.getPosition().z + 0.5)));
		(*it)->setLightPower(3.0);
		//(*it).setLightColor(glm::vec3(0.0, 0.0,1.0));

		int i = 0;
		int j = 0;
		int k = 0;
		for (std::list<Light*>::iterator it = lights.begin(); it != lights.end(); ++it)
		{
			lightpos[i++] = (*it)->getLightPosition().x;
			lightpos[i++] = (*it)->getLightPosition().y;
			lightpos[i++] = (*it)->getLightPosition().z;

			lightcolor[j++] = (*it)->getLightDiffuse().x;
			lightcolor[j++] = (*it)->getLightDiffuse().y;
			lightcolor[j++] = (*it)->getLightDiffuse().z;


			lightpow[k++] = (*it)->getLightPower();

			//printf("%f ",lightpow[k]);
		}

		glUniform3fv(light_position_wsID, numlights, &lightpos[0]);
		glUniform3fv(light_colorID, numlights, &lightcolor[0]);
		glUniform1fv(lightpowerID, numlights, &lightpow[0]);

		keyboard.update(window);
		if (keyboard.checkKeyDown(GLFW_KEY_Q)){
			glm::vec3 camPos = m_TPCamera.getPosition();
			glm::vec3 lookDir = m_TPCamera.getDirection();
			btRigidBody* sphere = m_BulletWorld.addSphere(0.2, camPos.x + lookDir.x,
				camPos.y + lookDir.y, camPos.z + lookDir.z, 1.0, JADE);
			sphere->setLinearVelocity(btVector3(40 * lookDir.x, 40 * lookDir.y, 40 * lookDir.z));
		}

		framecounter++;
		if (framecounter == 40)
		{
			framecounter = 0;
			glm::vec3 ppos = m_PlayerCube.getPosition();
			ObstacleManager::generateRandomObstacle(glm::vec3(ppos.x, 0.0 , ppos.z - 200.0), m_BulletWorld, SILVER);
		}

		if (keyboard.isPressed(GLFW_KEY_W))
		{
			m_PlayerCube.moveCube(GLFW_KEY_W);
		}
		if (keyboard.isPressed(GLFW_KEY_S))
		{
			m_PlayerCube.moveCube(GLFW_KEY_S);
		}
		if (keyboard.isPressed(GLFW_KEY_A))
		{
			m_PlayerCube.moveCube(GLFW_KEY_A);
		}
		if (keyboard.isPressed(GLFW_KEY_D))
		{
			m_PlayerCube.moveCube(GLFW_KEY_D);
		}

		m_BulletWorld.stepSimulate();

		//bodies.erase(remove_if(bodies.begin(), bodies.end(), ), bodies.end());
		std::list<RigidBodyTagMaterial*> bodies = m_BulletWorld.getBodies();
		

		for (std::list<RigidBodyTagMaterial*>::iterator it = bodies.begin(); it != bodies.end(); it++)
		{
			//if ((*it).tag == FLOOR)
			//	continue;
			Material temp_mat;
			temp_mat = (*it)->m_Material;
			glm::vec3 temp_amb = temp_mat.getMaterialAmbient();
			glm::vec3 temp_diff = temp_mat.getMaterialDiffuse();
			glm::vec3 temp_spec = temp_mat.getMaterialSpecular();
			glUniform3f(mat_ambID, temp_amb.x, temp_amb.y, temp_amb.z);
			glUniform3f(mat_diffID, temp_diff.x, temp_diff.y, temp_diff.z);
			glUniform3f(mat_specID, temp_spec.x, temp_spec.y, temp_spec.z);
			glUniform1f(mat_shinID, 128 * temp_mat.getMaterialShininess());

			btVector3 compos = (*it)->body->getCenterOfMassPosition();
			btCollisionShape* shape = (*it)->body->getCollisionShape();
			
			if ((*it)->body->getCollisionShape()->getShapeType() == SPHERE_SHAPE_PROXYTYPE)
			{
				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = getSphereModelMatrix((*it)->body);
				NormalMatrix = glm::transpose(glm::inverse(ViewMatrix * ModelMatrix));
				glUniformMatrix4fv(NMID, 1, GL_FALSE, &NormalMatrix[0][0]);
				glUniformMatrix4fv(MID, 1, GL_FALSE, &ModelMatrix[0][0]);
				sphere.render();
			}
			else if ((*it)->body->getCollisionShape()->getShapeType() == BOX_SHAPE_PROXYTYPE)
			{
				//printf("%f %f %f\n", bodies[i]->getCenterOfMassPosition().x(), bodies[i]->getCenterOfMassPosition().y(), bodies[i]->getCenterOfMassPosition().z());
				//int temp;
				//scanf("%d",&temp);
				//printf("|");
				ModelMatrix = glm::mat4(1.0);
				ModelMatrix = getBoxModelMatrix((*it)->body);
				NormalMatrix = glm::transpose(glm::inverse(ViewMatrix * ModelMatrix));
				glUniformMatrix4fv(NMID, 1, GL_FALSE, &NormalMatrix[0][0]);
				glUniformMatrix4fv(MID, 1, GL_FALSE, &ModelMatrix[0][0]);
				cube.render();
			}
		}

		glfwSwapInterval(1);
		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
	glfwWindowShouldClose(window) == 0);


	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}