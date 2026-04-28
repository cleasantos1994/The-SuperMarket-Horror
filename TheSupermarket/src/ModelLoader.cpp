#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "ModelLoader.h"
#include <SDL2/SDL.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>

void Mesh::Draw(const Shader& shader) {
    shader.SetInt("texture1", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

ModelLoader::ModelLoader(const std::string& path) {
    LoadModel(path);
}

void ModelLoader::Draw(const Shader& shader) {
    for (auto& mesh : meshes_) mesh.Draw(shader);
}

void ModelLoader::LoadModel(const std::string& path) {
    Assimp::Importer importer;
    
    // Using SDL_RWops to load the file into memory first for Assimp
    SDL_RWops* rw = SDL_RWFromFile(path.c_str(), "rb");
    if (!rw) {
        std::cerr << "ERROR::ASSIMP::FILE_NOT_FOUND: " << path << std::endl;
        return;
    }
    Sint64 size = SDL_RWsize(rw);
    std::vector<unsigned char> buffer(size);
    SDL_RWread(rw, buffer.data(), 1, size);
    SDL_RWclose(rw);

    const aiScene* scene = importer.ReadFileFromMemory(buffer.data(), size, 
                                                      aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return;
    }

    for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
        aiMesh* aiMesh = scene->mMeshes[i];
        Mesh mesh;
        mesh.textureID = 0;
        for (unsigned int j = 0; j < aiMesh->mNumVertices; j++) {
            mesh.vertices.push_back(aiMesh->mVertices[j].x);
            mesh.vertices.push_back(aiMesh->mVertices[j].y);
            mesh.vertices.push_back(aiMesh->mVertices[j].z);
            mesh.vertices.push_back(aiMesh->mNormals[j].x);
            mesh.vertices.push_back(aiMesh->mNormals[j].y);
            mesh.vertices.push_back(aiMesh->mNormals[j].z);
            if (aiMesh->mTextureCoords[0]) {
                mesh.vertices.push_back(aiMesh->mTextureCoords[0][j].x);
                mesh.vertices.push_back(aiMesh->mTextureCoords[0][j].y);
            } else {
                mesh.vertices.push_back(0.0f);
                mesh.vertices.push_back(0.0f);
            }
        }
        for (unsigned int j = 0; j < aiMesh->mNumFaces; j++) {
            aiFace face = aiMesh->mFaces[j];
            for (unsigned int k = 0; k < face.mNumIndices; k++)
                mesh.indices.push_back(face.mIndices[k]);
        }
        
        glGenVertexArrays(1, &mesh.VAO);
        glGenBuffers(1, &mesh.VBO);
        glGenBuffers(1, &mesh.EBO);

        glBindVertexArray(mesh.VAO);
        glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);
        glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size() * sizeof(float), &mesh.vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * sizeof(unsigned int), &mesh.indices[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

        glBindVertexArray(0);
        meshes_.push_back(mesh);
    }
}
