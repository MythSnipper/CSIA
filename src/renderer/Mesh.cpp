#include <Mesh.hpp>

Mesh::Mesh(float vertices[], int vertices_size){
    initializeOpenGLFunctions();
    vao.bind();

    vbo.fill(vertices, vertices_size, GL_STATIC_DRAW);

    setup_attributes();

    glEnableVertexAttribArray(0);

    vao.unbind();

    vertexCount = vertices_size / (8 * sizeof(float));
}
Mesh::Mesh(float vertices[], int vertices_size, uint32_t indices[], int indices_size){
    initializeOpenGLFunctions();
    ebo = std::make_unique<EBO>();

    vao.bind();

    vbo.fill(vertices, vertices_size, GL_STATIC_DRAW);
    ebo->fill(indices, indices_size, GL_STATIC_DRAW);

    setup_attributes();

    glEnableVertexAttribArray(0);

    vao.unbind();

    indexCount = indices_size / sizeof(uint32_t);
}
Mesh::Mesh(std::vector<float>& vertices){
    initializeOpenGLFunctions();
    vao.bind();

    vbo.fill(vertices.data(), vertices.size() * sizeof(float), GL_STATIC_DRAW);

    setup_attributes();

    glEnableVertexAttribArray(0);

    vao.unbind();

    vertexCount = vertices.size();
}
Mesh::Mesh(std::vector<float>& vertices, std::vector<uint32_t>& indices){
    initializeOpenGLFunctions();
    ebo = std::make_unique<EBO>();

    vao.bind();

    vbo.fill(vertices.data(), vertices.size() * sizeof(float), GL_STATIC_DRAW);
    ebo->fill(indices.data(), indices.size() * sizeof(uint32_t), GL_STATIC_DRAW);

    setup_attributes();

    glEnableVertexAttribArray(0);

    vao.unbind();

    indexCount = indices.size();
}
void Mesh::bind(){
    vao.bind();
}
void Mesh::unbind(){
    vao.unbind();
}
void Mesh::draw(){
    vao.bind();

    if(ebo){
        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
    }
    else{
        glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    }

    vao.unbind();
}
void Mesh::setup_attributes(){
    uint32_t stride = 8 * sizeof(float);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*) 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*) (6 * sizeof(float)));
    glEnableVertexAttribArray(2);
}
