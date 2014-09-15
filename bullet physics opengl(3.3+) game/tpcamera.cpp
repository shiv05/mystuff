#include "tpcamera.hpp"
#include "glm/gtc/matrix_transform.hpp"


void TPCamera::computeMatricesFromInputs(glm::vec3 position){

	cam_position.x = position.x;
	cam_position.z = position.z + 10;

	ProjectionMatrix = glm::perspective(50.0f, 4.0f / 3.0f, 0.1f, 1000.0f);
	// Camera matrix
	ViewMatrix = glm::lookAt(
		cam_position,           // Camera is here
		cam_position + cam_direction, // and looks here : at the same cam_cam_position, plus "cam_direction"
		glm::vec3(0.0,1.0,0.0)                  // Head is up (set to 0,-1,0 to look upside-down)
		);

}