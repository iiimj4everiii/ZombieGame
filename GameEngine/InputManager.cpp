#include "InputManager.h"

namespace GameEngine {

	InputManager::InputManager() : _mouseCoords(0.0f) {}

	InputManager::~InputManager()
	{
	}

	void InputManager::update()
	{
		//for (auto it = _keyMap.begin(); it != _keyMap.end(); it++)
		//{
		//	_previousKeyMap[it->first] = it->second;
		//}

		//This is c++ 11 for each loop.
		//Looping through _keyMap and copy it over to _previousKeyMap
		for (auto& it : _keyMap)
		{
			_previousKeyMap[it.first] = it.second;
		}
	}

	//Since event polling is slower, we should hold the KEYDOWN to be true unless we let go of the key.
	//Then the poll event will be KEYUP.
	//Since we are holding the value, the animation will look smoother
	void InputManager::pressKey(unsigned int keyID)
	{
		//Treat map as an array.
		//First check if keyID is in the map, if not then keyID will be created and then set to true
		_keyMap[keyID] = true;

		return;
	}

	void InputManager::releaseKey(unsigned int keyID)
	{
		_keyMap[keyID] = false;

		return;
	}

	bool InputManager::isKeyDown(unsigned int keyID)
	{
		auto it = _keyMap.find(keyID);

		if (it != _keyMap.end())
		{
			return it->second;
		}
		else
		{
			return false;
		}

		return false;
	}

	bool InputManager::wasKeyDown(unsigned int keyID)
	{
		auto it = _previousKeyMap.find(keyID);

		if (it != _previousKeyMap.end())
		{
			return it->second;
		}
		else
		{
			return false;
		}

		return false;
	}

	bool InputManager::isKeyPressed(unsigned int keyID)
	{
		//Check if it is pressed this frame but was not pressed last frame.
		if (isKeyDown(keyID) == true && wasKeyDown(keyID) == false)
		{
			return true;
		}

		return false;
	}

	void InputManager::setMouseCoords(float x, float y)
	{
		_mouseCoords.x = x;
		_mouseCoords.y = y;

		return;
	}
}