#include "galley.h"

Galley::Galley() {
	oar.loadModel();
	galley.loadModel();
	galleyMatrix = glm::mat4(1.0f);
}

void Galley::render(float leftOarsAngle, float rightOarsAngle, Camera camera, glm::vec4 vector, int direction) {
	move(direction, camera);
	glm::mat4 M = galleyMatrix;
	M = glm::rotate(M, PI, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 P = glm::perspective(50.0f * PI / 180.0f, 1.0f, 0.01f, 5000.0f);
	M = glm::scale(M, glm::vec3(0.01f, 0.01f, 0.01f));

	galley.draw(P, camera.getPosistion(), M, vector);
	glm::mat4 leftOarMatrix = M;
	glm::mat4 rightOarMatrix = glm::rotate(leftOarMatrix, PI, glm::vec3(0.0f, 1.0f, 0.0f));
	for (int i = 0; i < 7; i++) {
		if (i == 0) {
			leftOarMatrix = glm::translate(leftOarMatrix, glm::vec3(-169.0f, 88.0f, 53.0f));
			rightOarMatrix = glm::translate(rightOarMatrix, glm::vec3(-167.0f, 88.0f, -53.0f));
		}
		else {
			leftOarMatrix = glm::translate(leftOarMatrix, glm::vec3(0.0f, 0.0f + i * 1.3f, -97.0f + i * 1.3f));
			rightOarMatrix = glm::translate(rightOarMatrix, glm::vec3(0.0f, 0.0f + i * 1.3f, 97.0f));
		}
		glm::mat4 rotatedLeftOarMatrix, rotatedRightOarMatrix;
		rotatedLeftOarMatrix = oam::rotateLeftOars(leftOarsAngle, leftOarMatrix);
		rotatedRightOarMatrix = oam::rotateRightOars(rightOarsAngle, rightOarMatrix);
		oar.draw(P, camera.getPosistion(), rotatedRightOarMatrix, vector);
		oar.draw(P, camera.getPosistion(), rotatedLeftOarMatrix, vector);
	}
}

void Galley::move(int direction, Camera camera) {
	switch (direction)
	{
		case 1:
			galleyMatrix = glm::translate(galleyMatrix, glm::vec3(0.0f, 0.0f, 0.01f));
			break;
		case 2:
			galleyMatrix = glm::translate(galleyMatrix, glm::vec3(0.0f, 0.0f, -0.01f));
			break;
		case 3:
			galleyMatrix = glm::rotate(galleyMatrix,PI/1440, glm::vec3(0.0f, 1.0f, 0.0f));
			galleyMatrix = glm::translate(galleyMatrix, glm::vec3(0.0f, 0.0f, 0.005f));
			break;
		case 4:
			galleyMatrix = glm::rotate(galleyMatrix, -PI / 1440, glm::vec3(0.0f, 1.0f, 0.0f));
			galleyMatrix = glm::translate(galleyMatrix, glm::vec3(0.0f, 0.0f, 0.005f));
			break;
	default:
		break;
	}
}
