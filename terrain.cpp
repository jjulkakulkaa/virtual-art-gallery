#include "terrain.h"

Terrain::Terrain() {
	terrainModel.loadModel();
}

void Terrain::render(glm::mat4 camera, glm::vec4 vector) {
	glm::mat4 terrainMatrix = glm::mat4(1.0f); //Zainicjuj macierz modelu macierz? jednostkow?
	glm::mat4 P = glm::perspective(50.0f * PI / 180.0f, 1.0f, 0.01f, 5000.0f);  //Wylicz macierz rzutowania
	terrainMatrix = glm::translate(terrainMatrix, glm::vec3(25.0f,0.0f,-25.0f));
	terrainMatrix = glm::scale(terrainMatrix, glm::vec3(100.0f, 100.0f, 100.0f));

	terrainModel.draw(P, camera, terrainMatrix, vector);
}