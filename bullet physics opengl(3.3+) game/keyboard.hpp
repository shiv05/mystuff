#ifndef KEYBOARD_HPP
#define KEYBOARD_HPP

#define GLFW_DLL

// Include GLFW
#include <glfw3.h>

#include <vector>

#include "helper.hpp"

class Keyboard
{
private:
	std::vector<KeyState> keysUsed;
public:
	void addKey(unsigned int keyCode);
	void initKeyboard();
	bool checkKeyUp(unsigned int keyCode);
	bool checkKeyDown(unsigned int keyCode);
	bool isPressed(unsigned int keyCode);
	void update(GLFWwindow *window);
};

#endif