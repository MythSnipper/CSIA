#pragma once

#include <VertexShader.hpp>
#include <FragmentShader.hpp>

#include <glm/glm.hpp>

typedef int UniformID;

class ShaderProgram : protected QOpenGLFunctions_3_3_Core{
public:
    GLuint id = 0;
    //if shaders need to be owned by the program
    VertexShader* vertShader = nullptr;
    FragmentShader* fragShader = nullptr;

    ShaderProgram();
    ~ShaderProgram();
    void from_shaders(VertexShader* vertex_shader, FragmentShader* fragment_shader);
    void from_sources(const char* vert_source, const char* frag_source);
    void from_files(const char* vert_filename, const char* frag_filename);

    void set_bool(const char* name, std::initializer_list<int> values);
    void set_bool(UniformID uid, std::initializer_list<int> values);
    void set_int(const char* name, std::initializer_list<int> values);
    void set_int(UniformID uid, std::initializer_list<int> values);
    void set_float(const char* name, std::initializer_list<float> values);
    void set_float(UniformID uid, std::initializer_list<float> values);
    void set_mat4(const char* name, bool transpose, glm::mat4 matrix);
    void set_mat4(UniformID uid, bool transpose, glm::mat4 matrix);
    void activate();
};

