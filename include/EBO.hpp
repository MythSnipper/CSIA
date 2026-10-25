#pragma once

#include <QOpenGLFunctions_3_3_Core>

class EBO : protected QOpenGLFunctions_3_3_Core{
public:
    GLuint id = 0;

    EBO();
    ~EBO();
    EBO(uint32_t indices[], size_t indices_size, GLenum usage);
    void bind();
    void unbind();
    void fill(uint32_t indices[], size_t indices_size, GLenum usage);

};