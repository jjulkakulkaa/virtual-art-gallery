#include "camera.h"
#include "constants.h"

float angle = PI/90;
float speed = 0.7f;

Camera::Camera() {
	x = 0;
	y = 20.0f;
	z = -30.0f;
	pitch = 1.0f;
	position = glm::lookAt(glm::vec3(x, y, z), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, pitch, 0.0f));
}

void Camera::updatePosition(float newX, float newY, float newZ) {
	position = glm::lookAt(glm::vec3(newX, newY, newZ), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, pitch, 0.0f));
	x = newX;
	y = newY;
	z = newZ;
}

void Camera::updatePositionAndLookAtPoint(float x, float z) {

}

glm::mat4 Camera::getPosistion() {
	return position;
}

float Camera::getHeight() {
	return y;
}

float Camera::getWidth() {
	return x;
}

float Camera::getDepth() {
	return z;
}

void Camera::inverPitch() {
	pitch = -pitch;
	position = glm::lookAt(glm::vec3(x, y, z), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, pitch, 0.0f));
}

void Camera::moveLeft() {
	float newX = glm::cos(-angle) * x - glm::sin(-angle) * z;
	float newZ = glm::sin(-angle) * x + glm::cos(-angle) * z;
	updatePosition(newX, y, newZ);
}

void Camera::moveRight() {
	float newX = glm::cos(angle) * x - glm::sin(angle) * z;
	float newZ = glm::sin(angle) * x + glm::cos(angle) * z;
	updatePosition(newX, y, newZ);
}

void Camera::moveTop() {
	float newZ = glm::cos(angle) * z - glm::sin(angle) * y;
	float newY = glm::sin(angle) * z + glm::cos(angle) * y;
	updatePosition(x, newY, newZ);
}

void Camera::moveBottom() {
	float newZ = glm::cos(-angle) * z - glm::sin(-angle) * y;
	float newY = glm::sin(-angle) * z + glm::cos(-angle) * y;
	updatePosition(x, newY, newZ);
}

void Camera::moveCenter() {
	updatePosition(x, y, z + speed);
}

void Camera::moveBack() {
	updatePosition(x, y, z - speed);
}

void Camera::moveStraightDown(float distance) {
	updatePosition(x, y - distance, z);
}

void Camera::moveStraightUp(float distance) {
	updatePosition(x, y + distance, z);
}
