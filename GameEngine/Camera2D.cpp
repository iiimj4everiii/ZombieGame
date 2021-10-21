#include "Camera2D.h"

namespace GameEngine {
	Camera2D::Camera2D() :
		_screenWidth(480),
		_screenHeight(360),
		_update(true),
		_scale(1.0f),
		_position(0.0f, 0.0f),
		_cameraMatrix(1.0f),
		_orthoMatrix(1.0f)
	{}

	Camera2D::~Camera2D()
	{
	}

	void Camera2D::init(int screenWidth, int screenHeight)
	{
		_screenWidth = screenWidth;
		_screenHeight = screenHeight;
		_orthoMatrix = glm::ortho(0.0f, (float)_screenWidth, 0.0f, (float)_screenHeight);


		return;
	}

	void Camera2D::update()
	{
		if (_update)
		{
			//+_screenWidth / 2 and +_screenHeight / 2 will move the camera to the center of the sprite.
			glm::vec3 translate(-_position.x + _screenWidth / 2, -_position.y + _screenHeight / 2, 0.0f);
			_cameraMatrix = glm::translate(_orthoMatrix, translate);
			
			glm::vec3 scale(_scale, _scale, 0.0f);
			_cameraMatrix = glm::scale(glm::mat4(1.0f), scale) * _cameraMatrix;

			_update = false;
		}

		return;
	}

	glm::vec2 Camera2D::convertScreenToWorldCoords(glm::vec2 screenCoords)
	{
		//Invert y-coordinates
		screenCoords.y = _screenHeight - screenCoords.y;

		//Convert coordinate from (0,0) at the top left corner to (0,0) at the center.
		screenCoords -= glm::vec2(_screenWidth / 2, _screenHeight / 2);

		//Resizing the screen size from scaling.
		//If we set the screen size half the size, the effect is the same as if we zoomed in by a factor of 2.
		screenCoords /= _scale;

		//Translate with the camera position.
		screenCoords += _position;

		return screenCoords;
	}
}