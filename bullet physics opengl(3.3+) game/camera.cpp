// Include GLFW
#define GLFW_DLL
#include <glfw3.h>
extern GLFWwindow* window;

// Include GLM
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "camera.hpp"


// Initial horizontal angle : toward -Z
float horizontalAngle = 3.14f;
// Initial vertical angle : none
float verticalAngle = 0.0f;
// Initial Field of View
float initialFoV = 60.0f;

void Camera::computeMatricesFromInputs(){

	// glfwGetTime is called only once, the first time this function is called
	static double lastTime = glfwGetTime();

	// Compute time difference between current and last frame
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);

	// Get mouse cam_position
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	// Reset mouse cam_position for next frame
	glfwSetCursorPos(window, windowSize.x / 2, windowSize.y / 2);

	// Compute new orientation
	horizontalAngle += turnSpeed * float(windowSize.x / 2 - xpos);
	verticalAngle += turnSpeed * float(windowSize.y / 2 - ypos);

	// cam_direction : Spherical coordinates to Cartesian coordinates conversion
	cam_direction = glm::vec3(
		cos(verticalAngle) * sin(horizontalAngle),
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
		);

	// Right vector
	glm::vec3 right = glm::vec3(
		sin(horizontalAngle - 3.14f / 2.0f),
		0,
		cos(horizontalAngle - 3.14f / 2.0f)
		);

	// Up vector
	glm::vec3 up = glm::cross(right, cam_direction);

	// Move forward
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
		cam_position += cam_direction * deltaTime * moveSpeed;
	}
	// Move backward
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
		cam_position -= cam_direction * deltaTime * moveSpeed;
	}
	// Strafe right
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS){
		cam_position += right * deltaTime * moveSpeed;
	}
	// Strafe left
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS){
		cam_position -= right * deltaTime * moveSpeed;
	}

	float FoV = initialFoV;

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	//(float)windowSize.y / (float)windowSize.x
	ProjectionMatrix = glm::perspective(50.0f, 4.0f/3.0f , 0.1f, 1000.0f);
	// Camera matrix
	ViewMatrix = glm::lookAt(
		cam_position,           // Camera is here
		cam_position + cam_direction, // and looks here : at the same cam_cam_position, plus "cam_direction"
		up                  // Head is up (set to 0,-1,0 to look upside-down)
		);

	// For the next frame, the "last time" will be "now"
	lastTime = currentTime;
}