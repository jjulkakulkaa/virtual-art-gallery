/*
Niniejszy program jest wolnym oprogramowaniem; możesz go
rozprowadzać dalej i / lub modyfikować na warunkach Powszechnej
Licencji Publicznej GNU, wydanej przez Fundację Wolnego
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

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>
#include "shaderprogram.h"
#include "water.h"
#include "waterFrameBuffers.h"
#include "galley.h"
#include "camera.h"
#include "terrain.h"
#include "skybox.h"

float speed_x = 0;//[radiany/s]
float speed_y = 0;//[radiany/s]
float leftOarsSpeed = 0;
float rightOarsSpeed = 0;
float aspectRatio = 1;
float targetSpeedOfOars = PI / 2;
bool leftMouseButtonPressed = false;

Camera camera;

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
			speed_y = -PI;
		}
		if (key == GLFW_KEY_RIGHT) {
			speed_y = PI;
		}
		if (key == GLFW_KEY_UP) {
			speed_x = -PI;
		}
		if (key == GLFW_KEY_DOWN) {
			speed_x = PI;
		}
		if (key == GLFW_KEY_A) {
			leftOarsSpeed = targetSpeedOfOars;
		}
		if (key == GLFW_KEY_Z) {
			leftOarsSpeed = -targetSpeedOfOars;
		}
		if (key == GLFW_KEY_D) {
			rightOarsSpeed = targetSpeedOfOars;
		}
		if (key == GLFW_KEY_C) {
			rightOarsSpeed = -targetSpeedOfOars;
		}
		if (key == GLFW_KEY_W) {
			leftOarsSpeed = targetSpeedOfOars;
			rightOarsSpeed = targetSpeedOfOars;
		}
		if (key == GLFW_KEY_S) {
			leftOarsSpeed = -targetSpeedOfOars;
			rightOarsSpeed = -targetSpeedOfOars;
		}
	}
	if (action == GLFW_RELEASE) {
		if (key == GLFW_KEY_LEFT || key == GLFW_KEY_RIGHT) {
			speed_y = 0;
		}
		if (key == GLFW_KEY_UP || key == GLFW_KEY_DOWN) {
			speed_x = 0;
		}
		if (key == GLFW_KEY_A || key == GLFW_KEY_Z) {
			leftOarsSpeed = 0;
		}
		if (key == GLFW_KEY_D || key == GLFW_KEY_C) {
			rightOarsSpeed = 0;
		}
		if (key == GLFW_KEY_W || key == GLFW_KEY_S) {
			leftOarsSpeed = 0;
			rightOarsSpeed = 0;
		}
	}
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT)
	{
		if (action == GLFW_PRESS)
		{
			leftMouseButtonPressed = true;
		}
		else if (action == GLFW_RELEASE)
		{
			leftMouseButtonPressed = false;
		}
	}
}

void cursorPosCallback(GLFWwindow* window, double xPos, double yPos) {
	if (leftMouseButtonPressed) {
		glfwSetCursorPos(window, 512, 512);
		if (xPos > 512 + 16) camera.moveRight();
		if (xPos < 512 - 16) camera.moveLeft();
		if (yPos > 512 + 16) camera.moveBottom();
		if (yPos < 512 - 16) camera.moveTop();
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
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetScrollCallback(window, scrollCallback);
	glfwSetCursorPosCallback(window, cursorPosCallback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetWindowSizeCallback(window, windowResizeCallback);
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

	Water water;
	Galley galley;
	Terrain terrain;
	SkyBox skybox;
	

	//Główna pętla
	float angle_x = 0;
	float angle_y = 0; 
	float leftOarsAngle = PI * 100000;
	float rightOarsAngle = PI * 100000;
	glfwSetTime(0);

	//Utworzenie FrameBuffers dla wody
	waterFrameBuffers buffers(window);

	while (!glfwWindowShouldClose(window)) 
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glEnable(GL_CLIP_DISTANCE0); // włączenie gl_ClipDistance do obsługi odbicia


		angle_x += speed_x * glfwGetTime(); 
		angle_y += speed_y * glfwGetTime();
		leftOarsAngle += leftOarsSpeed * glfwGetTime();
		rightOarsAngle += rightOarsSpeed * glfwGetTime();
		glfwSetTime(0);


		buffers.bindReflectionFrameBuffer();
		float distance = 2 * camera.getHeight();
		camera.moveStraightDown(distance);
		camera.inverPitch();
		terrain.render(camera.getPosistion(), glm::vec4(0, 1, 0, 0));
		skybox.render(camera.getPosistion(), glm::vec4(0, 1, 0, 0));
		galley.render(leftOarsAngle, rightOarsAngle, camera.getPosistion(), glm::vec4(0, 1, 0, 0));
		camera.moveStraightUp(distance);
		camera.inverPitch();
		
		buffers.bindRefractionFrameBuffer();
		terrain.render(camera.getPosistion(), glm::vec4(0, -1, 0, 0));
		skybox.render(camera.getPosistion(), glm::vec4(0, 1, 0, 0));
		galley.render(leftOarsAngle, rightOarsAngle, camera.getPosistion(), glm::vec4(0, -1, 0, 0));

		glDisable(GL_CLIP_DISTANCE0);
		buffers.unbindCurrentFrameBuffer();
		terrain.render(camera.getPosistion(), glm::vec4(0, 1, 0, 0));
		galley.render(leftOarsAngle, rightOarsAngle, camera.getPosistion(), glm::vec4(0, 1, 0, 0));
		skybox.render(camera.getPosistion(), glm::vec4(0, 1, 0, 0));
		water.render(camera.getPosistion(), buffers); 
		

		glfwSwapBuffers(window); //Skopiuj bufor tylny do bufora przedniego
		glfwPollEvents();
	}

	//Sekcja zwalniania zasobów

	freeOpenGLProgram(window);

	buffers.cleanUp();

	glfwDestroyWindow(window); 
	glfwTerminate();
	exit(EXIT_SUCCESS);
}
