#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class waterFrameBuffers {
private:
	static const int REFLECTION_WIDTH = 256;
	static const int REFLECTION_HEIGHT = 256;

	static const int REFRACTION_WIDTH = 1024;
	static const int REFRACTION_HEIGHT = 1024;

	GLuint reflectionFrameBuffer;
	GLuint reflectionTexture;
	GLuint reflectionDepthBuffer;

	GLuint refractionFrameBuffer;
	GLuint refractionTexture;
	GLuint refractionDepthTexture;

	GLFWwindow* window;

public:
	waterFrameBuffers(GLFWwindow* window);

	void cleanUp();

	void unbindCurrentFrameBuffer();
	void bindReflectionFrameBuffer();
	void bindRefractionFrameBuffer();

	GLuint getReflectionTexture();
	GLuint getRefractionTexture();
	GLuint getRefractionDepthTexture();

	void initializeReflectionFrameBuffer();
	void initializeRefractionFrameBuffer();

private:
	
};

