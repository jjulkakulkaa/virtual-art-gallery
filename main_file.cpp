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
#include "constants.h"
#include "allmodels.h"
#include "lodepng.h"
#include "shaderprogram.h"
#include "customModel.h"
#include "water.h"
#include "oarAnimation.h"
#include "waterFrameBuffers.h"

float speed_x = 0;//[radiany/s]
float speed_y = 0;//[radiany/s]
float leftOarsSpeed = 0;
float rightOarsSpeed = 0;
float aspectRatio = 1;
float targetSpeedOfOars = PI / 2;

CustomModel Oar("./model/oar.obj");
CustomModel Galley("./model/galley.obj");
Water water;

//Procedura obsługi błędów
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

void windowResizeCallback(GLFWwindow* window, int width, int height) {
	if (height == 0) return;
	aspectRatio = (float)width / (float)height;
	glViewport(0, 0, width, height);
}

//Procedura inicjująca
void initOpenGLProgram(GLFWwindow* window) {
    initShaders();
	//************Tutaj umieszczaj kod, który należy wykonać raz, na początku programu************
	glClearColor(0, 0, 0, 1); //Ustaw kolor czyszczenia bufora kolorów
	glEnable(GL_DEPTH_TEST); //Włącz test głębokości na pikselach
	glfwSetKeyCallback(window, key_callback);
	glfwSetWindowSizeCallback(window, windowResizeCallback);
	Oar.loadModel();
	Galley.loadModel();
}


//Zwolnienie zasobów zajętych przez program
void freeOpenGLProgram(GLFWwindow* window) {
    freeShaders();
}

//Procedura rysująca zawartość sceny
void drawScene(GLFWwindow* window,float angle_x,float angle_y, float leftOarsAngle, float rightOarsAngle) {
	//************Tutaj umieszczaj kod rysujący obraz******************l
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Wyczyść bufor koloru i bufor głębokości
	glm::mat4 baseMatrix = glm::mat4(1.0f);
	baseMatrix = glm::scale(baseMatrix, glm::vec3(100.0f, 100.0f, 100.0f));
	glm::mat4 galleyMatrix = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
	galleyMatrix = glm::rotate(galleyMatrix, angle_y, glm::vec3(0.0f, 1.0f, 0.0f)); //Pomnóż macierz modelu razy macierz obrotu o kąt angle wokół osi Y
	glm::mat4 V = glm::lookAt(glm::vec3(0.0f, 10.0f, -15.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)); //Wylicz macierz widoku
	glm::mat4 P = glm::perspective(50.0f * PI / 180.0f, aspectRatio, 0.01f, 50.0f);  //Wylicz macierz rzutowania
	galleyMatrix = glm::scale(galleyMatrix, glm::vec3(0.01f, 0.01f, 0.01f));

	Galley.draw(P, V, galleyMatrix);
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
		Oar.draw(P, V, rotatedRightOarMatrix);
		Oar.draw(P, V, rotatedLeftOarMatrix);
	}
	water.draw(P, V, baseMatrix);


	glfwSwapBuffers(window); //Skopiuj bufor tylny do bufora przedniego
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



	//Główna pętla
	float angle_x = 0;
	float angle_y = 0; 
	float leftOarsAngle = PI * 100000;
	float rightOarsAngle = PI * 100000;
	glfwSetTime(0);

	//Utworzenie FrameBuffers dla wody
	waterFrameBuffers fbos(window);

	while (!glfwWindowShouldClose(window)) 
	{
		angle_x += speed_x * glfwGetTime(); 
		angle_y += speed_y * glfwGetTime();
		leftOarsAngle += leftOarsSpeed * glfwGetTime();
		rightOarsAngle += rightOarsSpeed * glfwGetTime();
		glfwSetTime(0);

		fbos.bindReflectionFrameBuffer();
		drawScene(window, angle_x, angle_y, leftOarsAngle, rightOarsAngle);
		fbos.unbindCurrentFrameBuffer();

		glfwPollEvents();
	}

	//Sekcja zwalniania zasobów

	freeOpenGLProgram(window);

	fbos.cleanUp();

	glfwDestroyWindow(window); 
	glfwTerminate();
	exit(EXIT_SUCCESS);
}
