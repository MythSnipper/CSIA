#pragma once

#include <QOpenGLFunctions_3_3_Core>
#include <QString>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//forward declare
class Mesh;
class Material;

//Renderer objects
struct Color{
    float r;
    float g;
    float b;
    float a = 1.0f;
};
struct ColorRGBA{
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};
typedef int UniformID;


class VertexShader : protected QOpenGLFunctions_3_3_Core{
public:
    GLuint id = 0;

    VertexShader();
    ~VertexShader();
    void from_source(const char* shader_source_addr);
    void from_file(const char* filename);
};
class FragmentShader : protected QOpenGLFunctions_3_3_Core{
public:
    GLuint id = 0;

    FragmentShader();
    ~FragmentShader();
    void from_source(const char* shader_source_addr);
    void from_file(const char* filename);
};
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


class VBO : protected QOpenGLFunctions_3_3_Core{
public:
    GLuint id = 0;

    VBO();
    ~VBO();
    VBO(float vertices[], size_t vertices_size, GLenum usage);
    void bind();
    void unbind();
    void fill(float vertices[], size_t vertices_size, GLenum usage);
};
class VAO : protected QOpenGLFunctions_3_3_Core{
public:
    GLuint id = 0;

    VAO();
    ~VAO();
    void bind();
    void unbind();

};
class EBO : protected QOpenGLFunctions_3_3_Core{
public:
    GLuint id = 0;

    EBO();
    ~EBO();
    EBO(uint32_t indices[], size_t indices_size, GLenum usage);
    void bind();
    void unbind();
    void fill(uint32_t indices[], size_t indices_size, GLenum usage);

};
class Texture2D : protected QOpenGLFunctions_3_3_Core{
public:
    GLuint id = 0;

    Texture2D(char* file_path, GLenum internal_format);
    Texture2D(uint8_t* data, uint32_t width, uint32_t height, GLenum image_format, GLenum internal_format);
    ~Texture2D();
    void fill(char* file_path, GLenum internal_format);
    void fill(uint8_t* data, uint32_t width, uint32_t height, GLenum image_format, GLenum internal_format);
    void bind();
    void bind_texture_unit(GLenum texture_unit);

private:
    int texture_width;
    int texture_height;
    int color_channels_count;
};

struct Transform{
    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 rotation = glm::vec3(0.0f);
    glm::vec3 scale = glm::vec3(1.0f);

    glm::mat4 get_matrix();
};

class Camera{
public:
    //position and orientation
    glm::vec3 position{4.0f, 3.0f, 5.0f};
    float yaw = -90.0f;
    float pitch = 0.0f;
    glm::vec3 front, right, up;

    glm::vec3 worldUp{0.0f, 1.0f, 0.0f};

    //projection
    float fov = 90.0f;
    float aspect_ratio = 1.0f;
    float near_plane = 0.1f;
    float far_plane = 100.0f;

    //movement
    float movementSpeed = 10.0f;
    float rotationSensitivity = 0.2f;

    //get matrices for rendering
    glm::mat4 get_view_matrix();
    glm::mat4 get_projection_matrix();


    //move camera
    void moveForward(float amount);
    void moveRight(float amount);
    void moveUp(float amount);

    //other operations
    void rotate(float yaw, float pitch);
    void zoom(float amount);

private: 
    glm::vec3 getForward();
    glm::vec3 getRight();
    glm::vec3 getUp();

};
class RenderObject{
public:
    Transform transform;

    bool wireframeMode = false;
    Mesh* mesh = nullptr;
    Material* material = nullptr;
};
class Mesh : protected QOpenGLFunctions_3_3_Core{
public:
    Mesh(float vertices[], int vertices_size);
    Mesh(float vertices[], int vertices_size, uint32_t indices[], int indices_size);
    Mesh(std::vector<float>& vertices);
    Mesh(std::vector<float>& vertices, std::vector<uint32_t>& indices);

    void bind();
    void unbind();
    void draw();
    void setup_attributes();

private:
    VAO vao;
    VBO vbo;
    std::unique_ptr<EBO> ebo; //optional

    unsigned int vertexCount = 0;
    unsigned int indexCount = 0;
};


class Material{
public:
    ShaderProgram* shader = nullptr;
    Texture2D* albedo = nullptr;    //never null, use the renderer's white texture for untextured materials

    glm::vec3 color = glm::vec3(1.0f);
    glm::vec2 tiling = glm::vec2(1.0f);
    float shininess = 32.0f;
    float specularStrength = 0.5f;

    void apply();
};

class DirectionalLight{
public:
    glm::vec3 direction = glm::normalize(glm::vec3(-0.4f, -1.0f, -0.3f));
    glm::vec3 color = glm::vec3(1.0f);
    float ambient = 0.15f;
};



//others
std::string readFile(const std::string& filename);
std::string readQFile(const QString& path);



