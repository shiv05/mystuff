#ifndef CAMERA_HPP
#define CAMERA_HPP

//class used to calculate view and projection matrices such that camera 
//is moved using arrow keys and its direction is changed using mouse


class Camera
{
	//size of the window
	glm::i16vec2 windowSize;
	//speed with which the camera moves and turns
	float moveSpeed, turnSpeed;
	glm::mat4 ViewMatrix;
	glm::mat4 ProjectionMatrix;
	glm::vec3 cam_position;
	glm::vec3 cam_direction;
	
	public:
	
	//computes view and projection matrices,must be called before getProjectionMatrix()
	//and getViewMatrix() is used
	void computeMatricesFromInputs();
	
	Camera(int x = 1024, int y = 768, float move_speed = 3.0, float turn_speed = 0.005)
	{
		windowSize.x = x;
		windowSize.y = y;
		moveSpeed = move_speed;
		turnSpeed = turn_speed;

		// Initial cam_position : on +Z
		cam_position = glm::vec3(0, 0, 5);
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