#version 330

//Zmienne jednorodne
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

//Atrybuty
layout (location=0) in vec4 vertex; //wspolrzedne wierzcholka w przestrzeni modelu
layout (location=1) in vec4 normal; //wektor normalny w wierzcholku

out vec4 clipSpace;

void main(void) {
    clipSpace = P*V*M*vec4(vertex.x, 0.0, vertex.y, 1.0);
    gl_Position= clipSpace;
}
