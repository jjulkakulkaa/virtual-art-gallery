#include "light.h"

Light::Light(std::vector<float> color, std::vector<float> position) {
	light_ambient = color;
	light_diffuse = color;
	light_specular = color;
	light_position = position;
}

void Light::init() {
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glLightfv(GL_LIGHT0, GL_POSITION, light_position.data());
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient.data());
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse.data());
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular.data());
}

std::vector<float> Light::getPosition() {
	return light_position;
}

std::vector<float> Light::getColor() {
	return light_diffuse;
}