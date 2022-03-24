#include "KeyInput.h"

KeyInput KeyInput::myInstance;
KeyInput::KeyInput()
{
	for (int i = 0; i < 512; ++i)
	{
		myKeys[i] = false;
	}
}
KeyInput& KeyInput::GetInstance()
{
	
	return myInstance;
}

bool KeyInput::GetKeyDown(Keys aKey)
{
	bool result = false;
	std::map<int, bool>::iterator it = myKeys.find(static_cast<int>(aKey));
	if (it != myKeys.end())
	{
		result = myKeys[static_cast<int>(aKey)];
	}

	return result;
}

void KeyInput::SetupKeyInputs(GLFWwindow* aWindow)
{
	glfwSetKeyCallback(aWindow, KeyInput::CallBack);
}

KeyInput::~KeyInput()
{

}

void KeyInput::SetIsKeyDown(int aKey, bool aIsDown)
{
	std::map<int, bool>::iterator it = myKeys.find(aKey);
	if (it != myKeys.end())
	{
		myKeys[aKey] = aIsDown;
	}
}

void KeyInput::CallBack(GLFWwindow* aWindow, int aKey, int aScanCode, int aAction, int aMods)
{
	KeyInput::GetInstance().SetIsKeyDown(aKey, aAction != GLFW_RELEASE);
}
