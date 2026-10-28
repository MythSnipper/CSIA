#pragma once

#include <QOpenGLFunctions_3_3_Core>
#include <QString>
#include <QTimer>
#include <QElapsedTimer>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <RenderObject.hpp>
#include <ShaderProgram.hpp>
#include <Camera.hpp>
#include <Mesh.hpp>
#include <Material.hpp>
#include <Scene.hpp>

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

class DirectionalLight{
public:
    glm::vec3 direction = glm::normalize(glm::vec3(-0.4f, -1.0f, -0.3f));
    glm::vec3 color = glm::vec3(1.0f);
    float ambient = 0.15f;
};

class GLRenderer : protected QOpenGLFunctions_3_3_Core{
public:
    void initialize();
    void resize(int pixelWidth, int pixelHeight);
    void render();
    void syncRenderObjects();
    void destroyGLResources();

    void setScene(Scene* scene);

    Camera camera;

private:
    void createGLResources();

    //drawing
    void drawRenderObject(std::unique_ptr<RenderObject>& object, const glm::mat4& view, const glm::mat4& projection);
    void drawFullscreenPass(ShaderProgram& shader);
    void drawGrid();

    //objects
    void createRenderObject(ID id, Mesh* mesh, Material* material);
    void updateRenderObject(ID id, const Transform& transform);
    void destroyRenderObject(ID id);

    //misc
    void printInfo();

    //material
    Material* createMaterial(); //creates a default working material


    int viewportWidth = 1;   //physical pixels, what gl_FragCoord uses
    int viewportHeight = 1;

    //renderer objects
    //All renderer objects should depend on Scene for reconstruction
    Scene* scene = nullptr;

    std::unique_ptr<Mesh> cube, sphere;
    
    std::unique_ptr<Texture2D> whiteTexture, checkerTexture;
    std::vector<std::unique_ptr<Material>> materials;
    DirectionalLight light;
    std::unique_ptr<ShaderProgram> objectShader, wireframePlaneShader;

    std::unordered_map<ID, std::unique_ptr<RenderObject>> objects;

    GLuint fullscreenVAO = 0; //VAO for drawing fullscreen shader passes

};

void generate_sphere(float radius, uint32_t sectors, uint32_t stacks, std::vector<float>& vertices, std::vector<uint32_t>& indices);
void generate_cube(float halfSize, std::vector<float>& vertices, std::vector<uint32_t>& indices);


