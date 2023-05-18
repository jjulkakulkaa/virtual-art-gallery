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
#include "myCube.h"
#include "constants.h"
#include "allmodels.h"
#include "lodepng.h"
#include "shaderprogram.h"
#include "customModel.h"

float speed_x = 0;//[radiany/s]
float speed_y = 0;//[radiany/s]
float leftOarsSpeed = 0;
float rightOarsSpeed = 0;
float aspectRatio = 1;
float targetSpeedOfOars = PI / 2;

CustomModel Oar("./model/oar.obj");
CustomModel Galley("./model/galley.obj");

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



glm::mat4 rotateRightOars(float angle, glm::mat4 modelMatrix) {

	angle = fmod(angle, PI);

	if (angle > 0 && angle <= PI / 6) {
		modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	else if (angle > PI / 6 && angle <= 2 * PI / 6) {
		modelMatrix = glm::rotate(modelMatrix, PI / 6, glm::vec3(0.0f, 1.0f, 0.0f));
		modelMatrix = glm::rotate(modelMatrix, angle - (PI / 6), glm::vec3(0.0f, 0.0f, 1.0f));
	}
	else if (angle > 2 * PI / 6 && angle <= 4 * PI / 6) {
		modelMatrix = glm::rotate(modelMatrix, PI / 6, glm::vec3(0.0f, 1.0f, 0.0f));
		modelMatrix = glm::rotate(modelMatrix, -(angle - (2 * PI / 6)), glm::vec3(0.0f, 1.0f, 0.0f));
		modelMatrix = glm::rotate(modelMatrix, PI / 6, glm::vec3(0.0f, 0.0f, 1.0f));
	}
	else if (angle > 4 * PI / 6 && angle <= 5 * PI / 6) {
		modelMatrix = glm::rotate(modelMatrix, -PI / 6, glm::vec3(0.0f, 1.0f, 0.0f));
		modelMatrix = glm::rotate(modelMatrix, PI/6 - (angle - (4 * PI / 6)), glm::vec3(0.0f, 0.0f, 1.0f));
	}
	else if (angle > 5 * PI / 6) {
		modelMatrix = glm::rotate(modelMatrix, -PI/6 + (angle - (5 * PI / 6)), glm::vec3(0.0f, 1.0f, 0.0f));
	}

	return modelMatrix;
}

glm::mat4 rotateLeftOars(float angle, glm::mat4 modelMatrix) {

	angle = fmod(angle, PI);

	if (angle > 0 && angle <= PI / 6) {
		modelMatrix = glm::rotate(modelMatrix, -angle, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	else if (angle > PI / 6 && angle <= 2 * PI / 6) {
		modelMatrix = glm::rotate(modelMatrix, -PI / 6, glm::vec3(0.0f, 1.0f, 0.0f));
		modelMatrix = glm::rotate(modelMatrix, angle - (PI / 6), glm::vec3(0.0f, 0.0f, 1.0f));
	}
	else if (angle > 2 * PI / 6 && angle <= 4 * PI / 6) {
		modelMatrix = glm::rotate(modelMatrix, -PI / 6, glm::vec3(0.0f, 1.0f, 0.0f));
		modelMatrix = glm::rotate(modelMatrix, (angle - (2 * PI / 6)), glm::vec3(0.0f, 1.0f, 0.0f));
		modelMatrix = glm::rotate(modelMatrix, PI / 6, glm::vec3(0.0f, 0.0f, 1.0f));
	}
	else if (angle > 4 * PI / 6 && angle <= 5 * PI / 6) {
		modelMatrix = glm::rotate(modelMatrix, PI / 6, glm::vec3(0.0f, 1.0f, 0.0f));
		modelMatrix = glm::rotate(modelMatrix, PI / 6 - (angle - (4 * PI / 6)), glm::vec3(0.0f, 0.0f, 1.0f));
	}
	else if (angle > 5 * PI / 6) {
		modelMatrix = glm::rotate(modelMatrix, PI / 6 - (angle - (5 * PI / 6)), glm::vec3(0.0f, 1.0f, 0.0f));
	}

	return modelMatrix;
}



//Procedura rysująca zawartość sceny
void drawScene(GLFWwindow* window,float angle_x,float angle_y, float leftOarsAngle, float rightOarsAngle) {
	//************Tutaj umieszczaj kod rysujący obraz******************l
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Wyczyść bufor koloru i bufor głębokości
	
	glm::mat4 galleyMatrix = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
	galleyMatrix = glm::rotate(galleyMatrix, angle_y, glm::vec3(0.0f, 1.0f, 0.0f)); //Pomnóż macierz modelu razy macierz obrotu o kąt angle wokół osi Y
	galleyMatrix = glm::rotate(galleyMatrix, angle_x, glm::vec3(0.0f, 0.0f, 1.0f)); //Pomnóż macierz modelu razy macierz obrotu o kąt angle wokół osi X
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
		rotatedLeftOarMatrix = rotateLeftOars(leftOarsAngle, leftOarMatrix);
		rotatedRightOarMatrix = rotateRightOars(rightOarsAngle, rightOarMatrix);
		Oar.draw(P, V, rotatedRightOarMatrix);
		Oar.draw(P, V, rotatedLeftOarMatrix);
	}


	glfwSwapBuffers(window); //Skopiuj bufor tylny do bufora przedniego
}


int main(void)
{
	GLFWwindow* window; //Wskaźnik na obiekt reprezentujący okno

	glfwSetErrorCallback(error_callback);//Zarejestruj procedurę obsługi błędów

	if (!glfwInit()) { //Zainicjuj bibliotekę GLFW
		fprintf(stderr, "Nie można zainicjować GLFW.\n");
		exit(EXIT_FAILURE);
	}

	window = glfwCreateWindow(1024, 1024, "OpenGL", NULL, NULL);  //Utwórz okno 500x500 o tytule "OpenGL" i kontekst OpenGL.

	if (!window) //Jeżeli okna nie udało się utworzyć, to zamknij program
	{
		fprintf(stderr, "Nie można utworzyć okna.\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window); //Od tego momentu kontekst okna staje się aktywny i polecenia OpenGL będą dotyczyć właśnie jego.
	glfwSwapInterval(1); //Czekaj na 1 powrót plamki przed pokazaniem ukrytego bufora

	if (glewInit() != GLEW_OK) { //Zainicjuj bibliotekę GLEW
		fprintf(stderr, "Nie można zainicjować GLEW.\n");
		exit(EXIT_FAILURE);
	}

	initOpenGLProgram(window); //Operacje inicjujące

	//Główna pętla
	float angle_x = 0; //zadeklaruj zmienną przechowującą aktualny kąt obrotu
	float angle_y = 0; //zadeklaruj zmienną przechowującą aktualny kąt obrotu
	float leftOarsAngle = PI * 100000;
	float rightOarsAngle = PI * 100000;
	glfwSetTime(0); //Wyzeruj licznik czasu
	while (!glfwWindowShouldClose(window)) //Tak długo jak okno nie powinno zostać zamknięte
	{
		angle_x += speed_x * glfwGetTime(); //Oblicz kąt o jaki obiekt obrócił się podczas poprzedniej klatki
		angle_y += speed_y * glfwGetTime(); //Oblicz kąt o jaki obiekt obrócił się podczas poprzedniej klatki
		leftOarsAngle += leftOarsSpeed * glfwGetTime();
		rightOarsAngle += rightOarsSpeed * glfwGetTime();
		glfwSetTime(0); //Wyzeruj licznik czasu
		drawScene(window,angle_x,angle_y, leftOarsAngle, rightOarsAngle); //Wykonaj procedurę rysującą
		glfwPollEvents(); //Wykonaj procedury callback w zalezności od zdarzeń jakie zaszły.
	}

	freeOpenGLProgram(window);

	glfwDestroyWindow(window); //Usuń kontekst OpenGL i okno
	glfwTerminate(); //Zwolnij zasoby zajęte przez GLFW
	exit(EXIT_SUCCESS);
}
