#pragma once

#include <QOpenGLWidget>
#include <QTimer>
#include <QElapsedTimer>
#include <QPoint>

//forward declare
class GLRenderer;

class QT_GLRenderer : public QOpenGLWidget{
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

    std::unique_ptr<GLRenderer> renderer;
    QMetaObject::Connection contextConnection;

    QTimer renderTimer;
    QElapsedTimer frameTimer;
    uint32_t FPS = 60; //fps setting

    bool moveForward = false;
    bool moveBackward = false;
    bool moveLeft = false;
    bool moveRight = false;
    bool moveUp = false;
    bool moveDown = false;

    QPoint lastMousePosition;
    bool rotating = false;
};


