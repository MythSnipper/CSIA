#include <EBO.hpp>


EBO::EBO(){
    initializeOpenGLFunctions();
    glGenBuffers(1, &id);
}
EBO::~EBO(){
    if(id != 0){
        puts("Log: EBO: Killing EBO");
        glDeleteBuffers(1, &id);
        id = 0;
    }
}
EBO::EBO(uint32_t indices[], size_t indices_size, GLenum usage){
    initializeOpenGLFunctions();
    glGenBuffers(1, &id);
    fill(indices, indices_size, usage);
}
void EBO::bind(){
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
}
void EBO::unbind(){
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
void EBO::fill(uint32_t indices[], size_t indices_size, GLenum usage){
    bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_size, indices, usage);
}

