#pragma once

#ifndef CUSTOM_MODEL_H
#define CUSTOM_MODEL_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <map>

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
#include "assimp_glm.h"

using namespace std;

GLuint readTexture(const char* filename);
typedef unsigned int uint;

#define MAX_BONE_INFLUENCE 4

struct Vertex {
    // position
    glm::vec4 Position;
    // normal
    glm::vec4 Normal;
    // texCoords
    glm::vec2 TexCoords;
    // tangent
    
    //bone indexes which will influence this vertex
    int m_BoneIDs[MAX_BONE_INFLUENCE];
    //weights from each bone
    float m_Weights[MAX_BONE_INFLUENCE];
};

struct BoneInfo
{
    /*id is index in finalBoneMatrices*/
    int id;

    /*offset matrix transforms vertex from model space to bone space*/
    glm::mat4 offset;

};

struct BasicMesh {
    int NumVertices = 0;
    vector<unsigned int> indices = vector<unsigned int>();
    vector<Vertex> vertices = vector<Vertex>();

    GLuint texture;
};

class AnimatedModel {
private:
    string modelPath;

    std::map<string, BoneInfo> m_BoneInfoMap;
    int m_BoneCounter = 0;

    void SetVertexBoneDataToDefault(Vertex& vertex);
    void SetVertexBoneData(Vertex& vertex, int boneID, float weight);
    void ExtractBoneWeightForVertices(std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene);
    
public:
    vector<BasicMesh> Meshes = vector<BasicMesh>();
    AnimatedModel(std::string filePath);

    void loadModel();

    void draw(glm::mat4 P, glm::mat4 V, glm::mat4 M);

    std::map<string, BoneInfo>& GetBoneInfoMap();
    int& GetBoneCount();
};

#endif
