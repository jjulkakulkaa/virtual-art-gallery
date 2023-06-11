#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "shaderprogram.h"
#include "lodepng.h"

using namespace std;

GLuint readTexture2(const char* filename);

struct BasicMeshM {
    int NumVertices = 0;
    vector<glm::vec4> vertices = vector<glm::vec4>();
    vector<glm::vec4> normals = vector<glm::vec4>();
    vector<glm::vec2> texCoords = vector<glm::vec2>();
    vector<unsigned int> indices = vector<unsigned int>();
    GLuint texture;
};

class CustomModel {
private:
    string modelPath;

public:
    vector<BasicMeshM> Meshes = vector<BasicMeshM>();
    CustomModel(std::string filePath);

    void loadModel();

    void draw(glm::mat4 P, glm::mat4 V, glm::mat4 M);
};