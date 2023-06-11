#include "customModelAnimated.h"
#include <stdbool.h>

GLuint readTexture(const char* filename) {
    GLuint tex;
    glActiveTexture(GL_TEXTURE0);

    //Wczytanie do pami?ci komputera
    std::vector<unsigned char> image;   //Alokuj wektor do wczytania obrazka
    unsigned width, height;   //Zmienne do których wczytamy wymiary obrazka
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

AnimatedModel::AnimatedModel(std::string filePath) {
    modelPath = filePath;
}

void AnimatedModel::loadModel() {
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
                BasicMesh meshStruct;
                meshStruct.NumVertices = mesh->mNumVertices;

                // Wczytanie wszystkich wierzcholkow, wektorow normalnych i wspolrzednych teksturowania
                for (int i = 0; i < mesh->mNumVertices; i++) {

                    Vertex vertexStruct;

                    SetVertexBoneDataToDefault(vertexStruct);

                    aiVector3D vertex = mesh->mVertices[i];
                    vertexStruct.Position = glm::vec4(vertex.x, vertex.y, vertex.z, 1.0f);
                    //cout << vertex.x << vertex.y << endl;
                    aiVector3D normal = mesh->mNormals[i];
                    vertexStruct.Normal = glm::vec4(normal.x, normal.y, normal.z, 0);
                    //cout << normal.x << normal.y << normal.z << endl;
                    aiVector3D texCoord = mesh->mTextureCoords[0][i];
                    vertexStruct.TexCoords = glm::vec2(texCoord.x, texCoord.y);

                    meshStruct.vertices.push_back(vertexStruct);

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
                    meshStruct.texture = readTexture(str.C_Str());
                }

                Meshes.push_back(meshStruct);
                ExtractBoneWeightForVertices(Meshes[numOfMesh].vertices, mesh, scene);
            }
        }
    }
}

void AnimatedModel::draw(glm::mat4 P, glm::mat4 V, glm::mat4 M) {
    for (int i = 0; i < Meshes.size(); i++) {

        GLuint bufferID;
        glGenBuffers(1, &bufferID);
        glBindBuffer(GL_ARRAY_BUFFER, bufferID);

        glUniformMatrix4fv(spAnimatedLambertTextured->u("P"), 1, false, glm::value_ptr(P));
        glUniformMatrix4fv(spAnimatedLambertTextured->u("V"), 1, false, glm::value_ptr(V));
        glUniformMatrix4fv(spAnimatedLambertTextured->u("M"), 1, false, glm::value_ptr(M));

        glBufferData(GL_ARRAY_BUFFER, Meshes[i].vertices.size() * sizeof(Vertex), &Meshes[i].vertices[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(spAnimatedLambertTextured->a("vertex"));
        glVertexAttribPointer(spAnimatedLambertTextured->a("vertex"), 4, GL_FLOAT, false, sizeof(Vertex), (void*)0);

        glEnableVertexAttribArray(spAnimatedLambertTextured->a("texCoord"));
        glVertexAttribPointer(spAnimatedLambertTextured->a("texCoord"), 2, GL_FLOAT, false, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

        glEnableVertexAttribArray(spAnimatedLambertTextured->a("normal"));
        glVertexAttribPointer(spAnimatedLambertTextured->a("normal"), 4, GL_FLOAT, false, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

        glEnableVertexAttribArray(spAnimatedLambertTextured->a("boneIds"));
        glVertexAttribIPointer(spAnimatedLambertTextured->a("boneIds"), 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, m_BoneIDs));

        glEnableVertexAttribArray(spAnimatedLambertTextured->a("weights"));
        glVertexAttribPointer(spAnimatedLambertTextured->a("weights"), 4, GL_FLOAT, false, sizeof(Vertex), (void*)offsetof(Vertex, m_Weights));

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, Meshes[i].texture);
        glUniform1i(spAnimatedLambertTextured->u("tex"), 0);

        glDrawElements(GL_TRIANGLES, Meshes[i].indices.size(), GL_UNSIGNED_INT, Meshes[i].indices.data());

        glDisableVertexAttribArray(spAnimatedLambertTextured->a("vertex"));
        glDisableVertexAttribArray(spAnimatedLambertTextured->a("texCoord"));
        glDisableVertexAttribArray(spAnimatedLambertTextured->a("normal"));
        glDisableVertexAttribArray(spAnimatedLambertTextured->a("boneIds"));
        glDisableVertexAttribArray(spAnimatedLambertTextured->a("weights"));
    }
}

void AnimatedModel::SetVertexBoneDataToDefault(Vertex& vertex) {
    for (int i = 0; i < MAX_BONE_INFLUENCE; i++)
    {
        vertex.m_BoneIDs[i] = -1;
        vertex.m_Weights[i] = 0.0f;
    }
}

void AnimatedModel::SetVertexBoneData(Vertex& vertex, int boneID, float weight) {
    for (int i = 0; i < MAX_BONE_INFLUENCE; ++i)
    {
        if (vertex.m_BoneIDs[i] < 0)
        {
            vertex.m_Weights[i] = weight;
            vertex.m_BoneIDs[i] = boneID;
            break;
        }
    }
}

void AnimatedModel::ExtractBoneWeightForVertices(std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene) {
    for (int boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex) {
        int boneID = -1;
        std::string boneName = mesh->mBones[boneIndex]->mName.C_Str();
        if (m_BoneInfoMap.find(boneName) == m_BoneInfoMap.end()) {
            BoneInfo newBoneInfo;
            newBoneInfo.id = m_BoneCounter;
            newBoneInfo.offset = AssimpToGLM::ConvertMatrixToGLMFormat(
                mesh->mBones[boneIndex]->mOffsetMatrix);
            m_BoneInfoMap[boneName] = newBoneInfo;
            boneID = m_BoneCounter;
            m_BoneCounter++;
        }
        else {
            boneID = m_BoneInfoMap[boneName].id;
        }
        assert(boneID != -1);
        auto weights = mesh->mBones[boneIndex]->mWeights;
        int numWeights = mesh->mBones[boneIndex]->mNumWeights;

        for (int weightIndex = 0; weightIndex < numWeights; ++weightIndex) {
            int vertexId = weights[weightIndex].mVertexId;
            float weight = weights[weightIndex].mWeight;
            assert(vertexId <= vertices.size());
            SetVertexBoneData(vertices[vertexId], boneID, weight);
        }
    }
}

std::map<string, BoneInfo>& AnimatedModel::GetBoneInfoMap() { return m_BoneInfoMap; }
int& AnimatedModel::GetBoneCount() { return m_BoneCounter; }
