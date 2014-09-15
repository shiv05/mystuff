#ifndef TPCAMERA_HPP
#define TPCAMERA_HP

// Include GLFW
#define GLFW_DLL
#include <glfw3.h>
extern GLFWwindow* window;

// Include GLM
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"



class TPCamera
{
	//size of the window
	glm::i16vec2 windowSize;
	glm::mat4 ViewMatrix;
	glm::mat4 ProjectionMatrix;
	glm::vec3 cam_position;
	glm::vec3 cam_direction;

public:

	//computes view and projection matrices,must be called before getProjectionMatrix()
	//and getViewMatrix() is used
	void computeMatricesFromInputs(glm::vec3 );

	TPCamera(int x = 1024, int y = 768, float move_speed = 3.0, float turn_speed = 0.005)
	{
		windowSize.x = x;
		windowSize.y = y;

		// Initial cam_position : on +Z
		cam_position = glm::vec3(0, -2.0, 5);
		cam_direction = glm::vec3(0, 0, -1);

	}

	glm::mat4 getViewMatrix()
	{
		return ViewMatrix;
	}

	glm::mat4 getProjectionMatrix()
	{
		return ProjectionMatrix;
	}

	glm::vec3 getPosition()
	{
		return cam_position;
	}

	glm::vec3 getDirection()
	{
		return cam_direction;
	}
};

#endif