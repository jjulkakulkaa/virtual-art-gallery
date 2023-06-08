/*
Niniejszy program jest wolnym oprogramowaniem; możesz go
rozprowadzać dalej i / lub modyfikować na warunkach Powszechnej
 cencji Publicznej GNU, wydanej przez Fundację Wolnego
Oprogramowania - według wersji 2 tej Licencji lub(według twojego
wyboru) którejś z późniejszych wersji.

Niniejszy program rozpowszechniany jest z nadzieją, iż będzie on
użyteczny - jednak BEZ JAKIEJKOLWIEK GWARANCJI, nawet domyślnej
gwarancji PRZYDATNOŚCI HANDLOWEJ albo PRZYDATNOŚCI DO OKREŚLONYCH
ZASTOSOWAŃ.W celu uzyskania bliższych informacji sięgnij do
Powszechnej Licencji Publicznej GNU.

Z pewnością wraz z niniejszym programem otrzymałeś też egzemplarz
Powszechnej Licencji Publicznej GNU(GNU General Public License);
jeśli nie - napisz do Free Software Foundation, Inc., 59 Temple
Place, Fifth Floor, Boston, MA  02110 - 1301  USA
*/

#define GLM_FORCE_RADIANS
#define NONE 0
#define FORWARD 1
#define BACKWARD 2
#define LEFT 3
#define RIGHT 4

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>
#include "shaderprogram.h"
#include "light.h"
#include "camera.h"
#include "customModel.h"

float aspectRatio = 1;

Camera camera;
Light light = Light({ 1.0f,1.0f,1.0f,1.0f }, { 0.0f,0.0f, 0.0f,1.0f });

CustomModel Maria(".\\models\\maria-theresia\\source\\maria.obj");
CustomModel artGallery(".\\models\\art-galery\\source\\art_galery.obj");
CustomModel rooms(".\\models\\rooms\\source\\rooms.obj");

void error_callback(int error, const char* description) {
	fputs(description, stderr);
}

void key_callback(
	GLFWwindow* window,
	int key,
	int scancode,
	int action,
	int mod
) {
	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_LEFT) {
			camera.moveLeft();
		}
		if (key == GLFW_KEY_RIGHT) {
			camera.moveRight();
		}
		if (key == GLFW_KEY_UP) {
			camera.moveTop();
		}
		if (key == GLFW_KEY_DOWN) {
			camera.moveBottom();
		}
	}
	if (action == GLFW_REPEAT) {
		if (key == GLFW_KEY_LEFT) {
			camera.moveLeft();
		}
		if (key == GLFW_KEY_RIGHT) {
			camera.moveRight();
		}
		if (key == GLFW_KEY_UP) {
			camera.moveTop();
		}
		if (key == GLFW_KEY_DOWN) {
			camera.moveBottom();
		}
	}
}

void scrollCallback(GLFWwindow* window, double xOffset, double yOffset) {
	if (yOffset > 0) {
		camera.moveCenter();
	}
	else if (yOffset < 0) {
		camera.moveBack();
	}
}

void windowResizeCallback(GLFWwindow* window, int width, int height) {
	if (height == 0) return;
	aspectRatio = (float)width / (float)height;
	glViewport(0, 0, width, height);
}

//Procedura inicjująca
void initOpenGLProgram(GLFWwindow* window) {
    initShaders();
	//************Tutaj umieszczaj kod, który należy wykonać raz, na początku programu************
	glClearColor(1, 0, 0, 1); //Ustaw kolor czyszczenia bufora kolorów
	glEnable(GL_DEPTH_TEST); //Włącz test głębokości na pikselach
	glfwSetKeyCallback(window, key_callback);
	glfwSetScrollCallback(window, scrollCallback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetWindowSizeCallback(window, windowResizeCallback);
	light.init();
	rooms.loadModel();
}


//Zwolnienie zasobów zajętych przez program
void freeOpenGLProgram(GLFWwindow* window) {
    freeShaders();
}

int main(void)
{
	GLFWwindow* window;
	glfwSetErrorCallback(error_callback);
	if (!glfwInit()) { 
		fprintf(stderr, "Nie można zainicjować GLFW.\n");
		exit(EXIT_FAILURE);
	}
	window = glfwCreateWindow(1024, 1024, "Galley", NULL, NULL);
	if (!window)
	{
		fprintf(stderr, "Nie można utworzyć okna.\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	if (glewInit() != GLEW_OK) { //Zainicjuj bibliotekę GLEW
		fprintf(stderr, "Nie można zainicjować GLEW.\n");
		exit(EXIT_FAILURE);
	}
	initOpenGLProgram(window); //Operacje inicjujące

	glfwSetTime(0);

	while (!glfwWindowShouldClose(window)) 
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glfwSetTime(0);

		glm::mat4 M = glm::mat4(1.0f);
		M = glm::scale(M, glm::vec3(2.2f, 2.2f, 2.2f));
		glm::mat4 P = glm::perspective(glm::radians(50.0f), 1.0f, 1.0f, 500.0f);
		glm::mat4 V = camera.getPosistion();

		rooms.draw(P, V, M);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//Sekcja zwalniania zasobów

	freeOpenGLProgram(window);

	//buffers.cleanUp();

	glfwDestroyWindow(window); 
	glfwTerminate();
	exit(EXIT_SUCCESS);
}
