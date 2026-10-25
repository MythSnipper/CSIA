#include <VBO.hpp>


VBO::VBO(){
    initializeOpenGLFunctions();
    glGenBuffers(1, &id);
}
VBO::~VBO(){
    if(id != 0){
        puts("Log: VBO: Killing VBO");
        glDeleteBuffers(1, &id);
        id = 0;
    }
}
VBO::VBO(float vertices[], size_t vertices_size, GLenum usage){
    initializeOpenGLFunctions();
    glGenBuffers(1, &id);
    fill(vertices, vertices_size, usage);
}
void VBO::bind(){
    glBindBuffer(GL_ARRAY_BUFFER, id);
}
void VBO::unbind(){
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void VBO::fill(float vertices[], size_t vertices_size, GLenum usage){
    bind();
    glBufferData(GL_ARRAY_BUFFER, vertices_size, vertices, usage);
}