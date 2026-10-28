#pragma once

#include <QOpenGLFunctions_3_3_Core>

class VertexShader : protected QOpenGLFunctions_3_3_Core{
public:
    GLuint id = 0;

    VertexShader();
    ~VertexShader();
    void from_source(const char* shader_source_addr);
    void from_file(const char* filename);
};