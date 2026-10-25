#pragma once

#include <VAO.hpp>
#include <VBO.hpp>
#include <EBO.hpp>

class Mesh : protected QOpenGLFunctions_3_3_Core{
public:
    Mesh(float vertices[], int vertices_size);
    Mesh(float vertices[], int vertices_size, uint32_t indices[], int indices_size);
    Mesh(std::vector<float>& vertices);
    Mesh(std::vector<float>& vertices, std::vector<uint32_t>& indices);

    void bind();
    void unbind();
    void draw();
    void setup_attributes();

private:
    VAO vao;
    VBO vbo;
    std::unique_ptr<EBO> ebo; //optional

    unsigned int vertexCount = 0;
    unsigned int indexCount = 0;
};
