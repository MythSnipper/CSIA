#include <FragmentShader.hpp>

#include <readfile.hpp>

FragmentShader::FragmentShader(){
    initializeOpenGLFunctions();
}
FragmentShader::~FragmentShader(){
    if(id != 0){
        puts("Log: FragmentShader: Killing FragmentShader");
        glDeleteShader(id);
        id = 0;
    }
}
void FragmentShader::from_source(const char* shader_source_addr){
    if(id != 0){
        puts("Log: FragmentShader: Killing old FragmentShader");
        glDeleteShader(id);
        id = 0;
    }

    id = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(id, 1, &shader_source_addr, NULL);
    glCompileShader(id);
    {
        int success;
        char infoLog[1024];
        glGetShaderiv(id, GL_COMPILE_STATUS, &success);
        if(!success){
            glGetShaderInfoLog(id, 1024, NULL, infoLog);
            printf("Err: FragmentShader: FragmentShader compilation failed\n\t%s\n", infoLog);
            throw std::runtime_error("^");
        }
        else{
            puts("Log: FragmentShader: FragmentShader compiled successfully");
        }
    }
}
void FragmentShader::from_file(const char* filename){
    std::string shader_source = readQFile(QString::fromUtf8(filename));
    this->from_source(shader_source.c_str());
}