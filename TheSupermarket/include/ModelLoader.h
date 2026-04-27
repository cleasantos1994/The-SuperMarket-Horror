#pragma once
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include "Shader.h"

struct Mesh {
    unsigned int VAO, VBO, EBO;
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    unsigned int textureID;

    void Draw(const Shader& shader);
};

class ModelLoader {
public:
    ModelLoader(const std::string& path);
    void Draw(const Shader& shader);

private:
    std::vector<Mesh> meshes_;
    void LoadModel(const std::string& path);
};
