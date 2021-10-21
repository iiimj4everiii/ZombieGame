#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace GameEngine {
	class Camera2D
	{
	public:
		Camera2D();
		~Camera2D();

		void init(int screenWidth, int screenHeight);
		void update();

		//Setters
		void setPosition(glm::vec2& newPosition)
		{
			_position = newPosition;
			_update = true;
		}

		void setScale(float newScale)
		{
			_scale = newScale;
			_update = true;
		}

		//Getters
		glm::vec2 getPosition() { return _position; }
		glm::vec2 convertScreenToWorldCoords(glm::vec2 screenCoords);
		float getScale() { return _scale; }
		glm::mat4 getCameraMatrix() { return _cameraMatrix; }

	private:
		int _screenWidth;
		int _screenHeight;
		bool _update;
		float _scale;
		glm::vec2 _position;
		glm::mat4 _cameraMatrix;
		glm::mat4 _orthoMatrix;
	};
}
