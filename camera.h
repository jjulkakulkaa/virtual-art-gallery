#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
private:
	float x, y, z;
	glm::mat4 position;

	void updatePosition(float x, float y, float z);
public:
	Camera();

	glm::mat4 getPosistion();
	float getHeight();
	float getWidth();
	float getDepth();

	void moveLeft();
	void moveRight();
	void moveTop();
	void moveBottom();
	void moveCenter();
	void moveBack();
};