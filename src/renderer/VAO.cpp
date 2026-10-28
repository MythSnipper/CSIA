#include <VAO.hpp>

VAO::VAO(){
    initializeOpenGLFunctions();
    glGenVertexArrays(1, &id);
}
VAO::~VAO(){
    if(id != 0){
        puts("Log: VAO: Killing VAO");
        glDeleteVertexArrays(1, &id);
        id = 0;
    }
}
void VAO::bind(){
    glBindVertexArray(id);
}
void VAO::unbind(){
    glBindVertexArray(0);
}