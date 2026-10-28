#pragma once

#include <QOpenGLFunctions_3_3_Core>

class VAO : protected QOpenGLFunctions_3_3_Core{
public:
    GLuint id = 0;

    VAO();
    ~VAO();
    void bind();
    void unbind();

};