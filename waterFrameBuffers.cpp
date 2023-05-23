#include "waterFrameBuffers.h"

waterFrameBuffers::waterFrameBuffers(GLFWwindow* window): window(window) {
	initializeReflectionFrameBuffer();
	initializeRefractionFrameBuffer();
}

void waterFrameBuffers::initializeReflectionFrameBuffer() {
		glGenFramebuffers(1, &reflectionFrameBuffer);
		glGenTextures(1, &reflectionTexture);
		glGenRenderbuffers(1, &reflectionDepthBuffer);
}

void waterFrameBuffers::initializeRefractionFrameBuffer() {
	glGenFramebuffers(1, &refractionFrameBuffer);
	glGenTextures(1, &refractionTexture);
	glGenTextures(1, &refractionDepthTexture);
}

void waterFrameBuffers::cleanUp() {
	glDeleteFramebuffers(1, &reflectionFrameBuffer);
	glDeleteTextures(1, &reflectionTexture);
	glDeleteRenderbuffers(1, &reflectionDepthBuffer);
	glDeleteFramebuffers(1, &refractionFrameBuffer);
	glDeleteTextures(1, &refractionTexture);
	glDeleteTextures(1, &refractionDepthTexture);
}

void waterFrameBuffers::unbindCurrentFrameBuffer() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLuint waterFrameBuffers::getReflectionTexture() {
	return reflectionTexture;
}

GLuint waterFrameBuffers::getRefractionTexture() {
	return refractionTexture;
}

GLuint waterFrameBuffers::getRefractionDepthTexture() {
	return refractionDepthTexture;
}

void waterFrameBuffers::bindReflectionFrameBuffer() {
	glBindFramebuffer(GL_FRAMEBUFFER, reflectionFrameBuffer);
	glBindTexture(GL_TEXTURE_2D, reflectionTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, REFLECTION_WIDTH, REFLECTION_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, reflectionTexture, 0);
}

void waterFrameBuffers::bindRefractionFrameBuffer() {
	glBindFramebuffer(GL_FRAMEBUFFER, refractionFrameBuffer);
	glBindTexture(GL_TEXTURE_2D, refractionTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, REFRACTION_WIDTH, REFRACTION_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, refractionTexture, 0);
}