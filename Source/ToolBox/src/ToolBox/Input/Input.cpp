#include "Input.h"

Input Input::myInstance;
std::pair<double, double> Input::myMousePosition;
GLFWwindow* Input::myWindow;
Input::Input()
{
	for (int i = 0; i < 512; ++i)
	{
		myKeys[i] = false;
	}
}
Input& Input::GetInstance()
{

	return myInstance;
}

bool Input::GetKeyDown(Keys aKey)
{
	bool result = false;
	std::map<int, bool>::iterator it = myKeys.find(static_cast<int>(aKey));
	if (it != myKeys.end())
	{
		result = myKeys[static_cast<int>(aKey)];
	}

	return result;
}

void Input::SetupKeyInputs(GLFWwindow* aWindow)
{
	glfwSetKeyCallback(aWindow, Input::CallBack);
	glfwSetCursorPosCallback(aWindow, Input::MouseCallback);
	myWindow = aWindow;
}

bool Input::OnMouseMove()
{
	return false;
} 

bool Input::GetMouseButtonDown(int aButton)
{
	if (glfwGetMouseButton(myWindow, aButton) == GLFW_PRESS)
	{
		return true;
	}
	return false;
}


Input::~Input()
{

}

void Input::SetIsKeyDown(int aKey, bool aIsDown)
{
	std::map<int, bool>::iterator it = myKeys.find(aKey);
	if (it != myKeys.end())
	{
		myKeys[aKey] = aIsDown;
	}
}

void Input::MouseCallback(GLFWwindow* aWindow, double aXpos, double aYpos)
{
	myMousePosition.first = aXpos;
	myMousePosition.second = aYpos;
}

void Input::CallBack(GLFWwindow* aWindow, int aKey, int aScanCode, int aAction, int aMods)
{
	Input::GetInstance().SetIsKeyDown(aKey, aAction != GLFW_RELEASE);
}
