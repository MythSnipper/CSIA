#pragma once

#include <GLRenderer.hpp>

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QTimer>
#include <QElapsedTimer>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>

#include <memory>
#include <vector>

//forward declare
class VertexShader;
class FragmentShader;
class ShaderProgram;
class VBO;
class VAO;
class RenderObject;

class QT_GLRenderer : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core{
public:
    explicit QT_GLRenderer(QWidget* parent = nullptr);
    ~QT_GLRenderer();

protected:
    void initializeGL() override;
    void resizeGL(int width, int height) override;
    void paintGL() override;

    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;

    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;

private:
    void updateCamera(float deltaTime);
    void createGLResources();
    void destroyGLResources();
    void drawRenderObject(std::unique_ptr<RenderObject>& object);
    void printInfo();
    void applySettings();

    //settings
    bool wireframe_mode = false;
    bool depth_test = true;
    uint32_t FPS = 60;

    //renderer objects
    std::unique_ptr<Mesh> mesh;
    std::unique_ptr<Mesh> mesh2;
    std::unique_ptr<ShaderProgram> objectShader;
    std::unique_ptr<ShaderProgram> wireframePlaneShader;

    Camera camera;
    std::vector<std::unique_ptr<RenderObject>> objects;

    QTimer renderTimer;
    QElapsedTimer frameTimer;

    //input states
    //keyboard
    bool moveForward = false;
    bool moveBackward = false;
    bool moveLeft = false;
    bool moveRight = false;
    bool moveUp = false;
    bool moveDown = false;

    //mouse
    QPoint lastMousePosition;
    bool rotating = false;
    
};

