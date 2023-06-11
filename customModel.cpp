#include "customModel.h"
#include <stdbool.h>

GLuint readTexture2(const char* filename) {
    GLuint tex;
    glActiveTexture(GL_TEXTURE0);

    //Wczytanie do pami?ci komputera
    std::vector<unsigned char> image;   //Alokuj wektor do wczytania obrazka
    unsigned width, height;   //Zmienne do kt?rych wczytamy wymiary obrazka
    //Wczytaj obrazek
    unsigned error = lodepng::decode(image, width, height, filename);

    //Import do pami?ci karty graficznej
    glGenTextures(1, &tex); //Zainicjuj jeden uchwyt
    glBindTexture(GL_TEXTURE_2D, tex); //Uaktywnij uchwyt
    //Wczytaj obrazek do pami?ci KG skojarzonej z uchwytem
    glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0,
        GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)image.data());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return tex;
}

CustomModel::CustomModel(std::string filePath) {
    modelPath = filePath;
}

void CustomModel::loadModel() {
    {
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(
            modelPath,
            aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            std::cout << "Failed to load model: " << importer.GetErrorString() << std::endl;
            return;
        }
        if (scene->HasMeshes()) {
            for (int numOfMesh = 0; numOfMesh < scene->mNumMeshes; numOfMesh++) {
                aiMesh* mesh = scene->mMeshes[numOfMesh];
                BasicMeshM meshStruct;
                meshStruct.NumVertices = mesh->mNumVertices;
                // Wczytanie wszystkich wierzcholkow, wektorow normalnych i wspolrzednych teksturowania
                for (int i = 0; i < mesh->mNumVertices; i++) {

                    aiVector3D vertex = mesh->mVertices[i];
                    meshStruct.vertices.push_back(glm::vec4(vertex.x, vertex.y, vertex.z, 1));
                    //cout << vertex.x << vertex.y << endl;
                    aiVector3D normal = mesh->mNormals[i];
                    meshStruct.normals.push_back(glm::vec4(normal.x, normal.y, normal.z, 0));
                    //cout << normal.x << normal.y << normal.z << endl;
                    aiVector3D texCoord = mesh->mTextureCoords[0][i];
                    meshStruct.texCoords.push_back(glm::vec2(texCoord.x, texCoord.y));
                }
                // Wczytanie faces
                for (int i = 0; i < mesh->mNumFaces; i++) {
                    aiFace& face = mesh->mFaces[i];
                    for (int j = 0; j < face.mNumIndices; j++) {
                        meshStruct.indices.push_back(face.mIndices[j]);
                    }
                }

                aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

                for (int i = 0; i < material->GetTextureCount(aiTextureType_DIFFUSE); i++) {
                    aiString str;
                    material->GetTexture(aiTextureType_DIFFUSE, i, &str);
                    meshStruct.texture = readTexture2(str.C_Str());
                }

                Meshes.push_back(meshStruct);
            }
        }
    }
}

void CustomModel::draw(glm::mat4 P, glm::mat4 V, glm::mat4 M) {
    for (int i = 0; i < Meshes.size(); i++) {
        spLambertTextured->use();
        glUniformMatrix4fv(spLambertTextured->u("P"), 1, false, glm::value_ptr(P));
        glUniformMatrix4fv(spLambertTextured->u("V"), 1, false, glm::value_ptr(V));
        glUniformMatrix4fv(spLambertTextured->u("M"), 1, false, glm::value_ptr(M));

        glEnableVertexAttribArray(spLambertTextured->a("vertex"));
        glVertexAttribPointer(spLambertTextured->a("vertex"), 4, GL_FLOAT, false, 0, Meshes[i].vertices.data());

        glEnableVertexAttribArray(spLambertTextured->a("texCoord"));
        glVertexAttribPointer(spLambertTextured->a("texCoord"), 2, GL_FLOAT, false, 0, Meshes[i].texCoords.data());

        glEnableVertexAttribArray(spLambertTextured->a("normal"));
        glVertexAttribPointer(spLambertTextured->a("normal"), 4, GL_FLOAT, false, 0, Meshes[i].normals.data());

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, Meshes[i].texture);
        glUniform1i(spLambertTextured->u("tex"), 0);

        glDrawElements(GL_TRIANGLES, Meshes[i].indices.size(), GL_UNSIGNED_INT, Meshes[i].indices.data());

        glDisableVertexAttribArray(spLambertTextured->a("vertex"));
        glDisableVertexAttribArray(spLambertTextured->a("texCoord"));
        glDisableVertexAttribArray(spLambertTextured->a("normal"));
    }
}