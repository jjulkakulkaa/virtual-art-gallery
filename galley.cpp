#include "galley.h"

Galley::Galley()
{
	oar.loadModel();
	galley.loadModel();
}

void Galley::render(float leftOarsAngle, float rightOarsAngle, glm::mat4 camera) {
	glm::mat4 galleyMatrix = glm::mat4(1.0f); //Zainicjuj macierz modelu macierz? jednostkow?
	galleyMatrix = glm::rotate(galleyMatrix, PI, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 P = glm::perspective(50.0f * PI / 180.0f, 1.0f, 0.01f, 50.0f);  //Wylicz macierz rzutowania
	galleyMatrix = glm::scale(galleyMatrix, glm::vec3(0.01f, 0.01f, 0.01f));

	galley.draw(P, camera, galleyMatrix);
	glm::mat4 leftOarMatrix = galleyMatrix;
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
		oar.draw(P, camera, rotatedRightOarMatrix);
		oar.draw(P, camera, rotatedLeftOarMatrix);
	}
}
