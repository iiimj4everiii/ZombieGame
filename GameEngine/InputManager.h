#pragma once
#include <unordered_map>
#include <glm/glm.hpp>

namespace GameEngine {
	class InputManager
	{
	public:
		InputManager();
		~InputManager();

		void update();

		void pressKey(unsigned int keyID);
		void releaseKey(unsigned int keyID);

		//Returns true if the key is held down.
		bool isKeyDown(unsigned int keyID);

		//Returns true if the key is just pressed.
		bool isKeyPressed(unsigned int keyID);

		void setMouseCoords(float x, float y);

		//Use const to make the class unchangeable.
		//Usually getters will use const
		glm::vec2 getMouseCoords() const { return _mouseCoords; }

	private:
		bool wasKeyDown(unsigned int keyID);

		std::unordered_map<unsigned int, bool> _keyMap;
		std::unordered_map<unsigned int, bool> _previousKeyMap;
		glm::vec2 _mouseCoords;
	};
}
