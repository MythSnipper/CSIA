#include <VertexShader.hpp>

#include <readfile.hpp>

VertexShader::VertexShader(){
    initializeOpenGLFunctions();
}
VertexShader::~VertexShader(){
    if(id != 0){
        puts("Log: VertexShader: Killing VertexShader");
        glDeleteShader(id);
        id = 0;
    }
}
void VertexShader::from_source(const char* shader_source_addr){
    if(id != 0){
        puts("Log: VertexShader: Killing old VertexShader");
        glDeleteShader(id);
        id = 0;
    }

    id = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(id, 1, &shader_source_addr, NULL);
    glCompileShader(id);
    {
        int success;
        char infoLog[1024];
        glGetShaderiv(id, GL_COMPILE_STATUS, &success);
        if(!success){
            glGetShaderInfoLog(id, 1024, NULL, infoLog);
            printf("Err: VertexShader: VertexShader compilation failed\n\t%s\n", infoLog);
            throw std::runtime_error("^");
        }
        else{
            puts("Log: VertexShader: VertexShader compiled successfully");
        }
    }
}
void VertexShader::from_file(const char* filename){
    std::string shader_source = readQFile(QString::fromUtf8(filename));
    this->from_source(shader_source.c_str());
}

