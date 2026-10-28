#pragma once

#include <QOpenGLFunctions_3_3_Core>

class VBO : protected QOpenGLFunctions_3_3_Core{
public:
    GLuint id = 0;

    VBO();
    ~VBO();
    VBO(float vertices[], size_t vertices_size, GLenum usage);
    void bind();
    void unbind();
    void fill(float vertices[], size_t vertices_size, GLenum usage);
};

