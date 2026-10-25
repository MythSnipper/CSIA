#pragma once

#include <QOpenGLFunctions_3_3_Core>

class FragmentShader : protected QOpenGLFunctions_3_3_Core{
public:
    GLuint id = 0;

    FragmentShader();
    ~FragmentShader();
    void from_source(const char* shader_source_addr);
    void from_file(const char* filename);
};