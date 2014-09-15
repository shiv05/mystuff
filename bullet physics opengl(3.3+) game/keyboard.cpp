#include "keyboard.hpp"

void Keyboard::addKey(unsigned int keycode)
{
	KeyState temp;
	temp.keyCode = keycode;
	temp.previousState = false;
	temp.currentState = false;
	keysUsed.push_back(temp);
}

void Keyboard::initKeyboard()
{
	addKey(GLFW_KEY_Q);
	addKey(GLFW_KEY_W);
	addKey(GLFW_KEY_A);
	addKey(GLFW_KEY_D);
	addKey(GLFW_KEY_S);
}

void Keyboard::update(GLFWwindow *window)
{
	for (int i = 0; i < keysUsed.size(); i++)
	{
		keysUsed[i].previousState = keysUsed[i].currentState;
		if (glfwGetKey(window, keysUsed[i].keyCode) == GLFW_PRESS)
			keysUsed[i].currentState = true;
		else
			keysUsed[i].currentState = false;
	}
}

bool Keyboard::checkKeyUp(unsigned int keyCode)
{
	for (int i = 0; i < keysUsed.size(); i++)
	{
		if (keysUsed[i].keyCode == keyCode)
		{
			if (keysUsed[i].previousState == true && keysUsed[i].currentState == false)
				return true;
			else
				return false;
		}
	}
}

bool Keyboard::checkKeyDown(unsigned int keyCode)
{
	for (int i = 0; i < keysUsed.size(); i++)
	{
		if (keysUsed[i].keyCode == keyCode)
		{
			if (keysUsed[i].previousState == false && keysUsed[i].currentState == true)
				return true;
			else
				return false;
		}
	}
}

bool Keyboard::isPressed(unsigned int keyCode)
{
	for (int i = 0; i < keysUsed.size(); i++)
	{
		if (keysUsed[i].keyCode == keyCode)
		{
			if (keysUsed[i].currentState == true)
				return true;
			else
				return false;
		}
	}
	return false;
}
