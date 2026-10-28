#include <ShaderProgram.hpp>

#include <glm/gtc/type_ptr.hpp>

ShaderProgram::ShaderProgram(){
    initializeOpenGLFunctions();
}
ShaderProgram::~ShaderProgram(){
    if(id != 0){
        puts("Log: ShaderProgram: Killing ShaderProgram");
        glDeleteProgram(id);
        id = 0;
    }

    if(vertShader != nullptr){
        puts("Log: ShaderProgram: Killing VertexShader");
        delete vertShader;
        vertShader = nullptr;
    }
    if(fragShader != nullptr){
        puts("Log: ShaderProgram: Killing FragmentShader");
        delete fragShader;
        fragShader = nullptr;
    }
}
void ShaderProgram::from_shaders(VertexShader* vertex_shader, FragmentShader* fragment_shader){
    if(id != 0){
        puts("Log: ShaderProgram: Killing old ShaderProgram");
        glDeleteProgram(id);
        id = 0;
    }

    id = glCreateProgram();
    glAttachShader(id, vertex_shader->id);
    glAttachShader(id, fragment_shader->id);
    glLinkProgram(id);
    {
        int success;
        char infoLog[1024];
        glGetProgramiv(id, GL_LINK_STATUS, &success);
        if(!success){
            glGetProgramInfoLog(id, 1024, NULL, infoLog);
            printf("Err: ShaderProgram: ShaderProgram link failed\n\t%s\n", infoLog);
            throw std::runtime_error(" ^ ^");
        }
        else{
            puts("Log: ShaderProgram: ShaderProgram linked successfully");
        }
    }
}
void ShaderProgram::from_sources(const char* vert_source, const char* frag_source){
    if(id != 0){
        puts("Log: ShaderProgram: Killing old ShaderProgram");
        glDeleteProgram(id);
        id = 0;
    }

    if(vertShader != nullptr){
        puts("Log: ShaderProgram: Killing old VertexShader");
        delete vertShader;
        vertShader = nullptr;
    }
    if(fragShader != nullptr){
        puts("Log: ShaderProgram: Killing old FragmentShader");
        delete fragShader;
        fragShader = nullptr;
    }

    vertShader = new VertexShader();
    vertShader->from_source(vert_source);
    fragShader = new FragmentShader();
    fragShader->from_source(frag_source);

    this->from_shaders(vertShader, fragShader);
}
void ShaderProgram::from_files(const char* vert_filename, const char* frag_filename){
    if(id != 0){
        glDeleteProgram(id);
        id = 0;
    }

    if(vertShader != nullptr){
        puts("Log: ShaderProgram: Killing old VertexShader");
        delete vertShader;
        vertShader = nullptr;
    }
    if(fragShader != nullptr){
        puts("Log: ShaderProgram: Killing old FragmentShader");
        delete fragShader;
        fragShader = nullptr;
    }

    vertShader = new VertexShader();
    vertShader->from_file(vert_filename);
    fragShader = new FragmentShader();
    fragShader->from_file(frag_filename);

    this->from_shaders(vertShader, fragShader);
}
void ShaderProgram::set_bool(const char* name, std::initializer_list<int> values){
    set_int(name, values);
}
void ShaderProgram::set_bool(UniformID uid, std::initializer_list<int> values){
    set_int(uid, values);
}
void ShaderProgram::set_int(const char* name, std::initializer_list<int> values){
    set_int(glGetUniformLocation(id, name), values);
}
void ShaderProgram::set_int(UniformID uid, std::initializer_list<int> values){
    const int* jump = values.begin();
    switch(values.size()){
        case 1:
            glUniform1i(uid, jump[0]);
        break;
        case 2:
            glUniform2i(uid, jump[0], jump[1]);
        break;
        case 3:
            glUniform3i(uid, jump[0], jump[1], jump[2]);
        break;
        case 4:
            glUniform4i(uid, jump[0], jump[1], jump[2], jump[3]);
        break;
        default:
            printf("Err: ShaderProgram: Invalid number of values to set uniform. Expected 1-4, got %lu\n", values.size());
            throw std::runtime_error("^");
    }
}
void ShaderProgram::set_float(const char* name, std::initializer_list<float> values){
    set_float(glGetUniformLocation(id, name), values);
}
void ShaderProgram::set_float(UniformID uid, std::initializer_list<float> values){
    const float* jump = values.begin();
    switch(values.size()){
        case 1:
            glUniform1f(uid, jump[0]);
        break;
        case 2:
            glUniform2f(uid, jump[0], jump[1]);
        break;
        case 3:
            glUniform3f(uid, jump[0], jump[1], jump[2]);
        break;
        case 4:
            glUniform4f(uid, jump[0], jump[1], jump[2], jump[3]);
        break;
        default:
            printf("Err: ShaderProgram: Invalid number of values to set uniform. Expected 1-4, got %lu\n", values.size());
            throw std::runtime_error("^");
    }
}
void ShaderProgram::set_mat4(const char* name, bool transpose, glm::mat4 matrix){
    set_mat4(glGetUniformLocation(id, name), transpose, matrix);
}
void ShaderProgram::set_mat4(UniformID uid, bool transpose, glm::mat4 matrix){
    glUniformMatrix4fv(uid, 1, (transpose) ? GL_TRUE : GL_FALSE, glm::value_ptr(matrix));
}
void ShaderProgram::activate(){
    glUseProgram(id);
}
