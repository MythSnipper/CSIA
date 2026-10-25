#include "QT_GLRenderer.hpp"

#include <GLRenderer.hpp>

QT_GLRenderer::QT_GLRenderer(QWidget* parent) : QOpenGLWidget(parent), renderer(std::make_unique<GLRenderer>()){
    setFocusPolicy(Qt::StrongFocus);

    connect(&renderTimer, &QTimer::timeout, this, [this](){
        float deltaTime = frameTimer.restart() / 1000.0f;
        updateCamera(deltaTime);
        update();
    });

    renderTimer.setInterval(1000 / FPS);
    frameTimer.start();
    renderTimer.start();
}

QT_GLRenderer::~QT_GLRenderer(){
    QObject::disconnect(contextConnection);

    //member destructors run before ~QOpenGLWidget destroys the context, so free GL objects now
    if(isValid()){
        makeCurrent();
        renderer->destroyGLResources();
        doneCurrent();
    }
}

void QT_GLRenderer::initializeGL(){
    QObject::disconnect(contextConnection);
    contextConnection = connect(context(), &QOpenGLContext::aboutToBeDestroyed, this, [this](){
        makeCurrent();
        renderer->destroyGLResources();
        doneCurrent();
    }, Qt::DirectConnection);

    renderer->initialize();
    update();
}

void QT_GLRenderer::resizeGL(int newWidth, int newHeight){
    const double ratio = devicePixelRatioF();
    renderer->resize((int)(newWidth * ratio), (int)(newHeight * ratio));
}

void QT_GLRenderer::paintGL(){
    renderer->render();
}

void QT_GLRenderer::updateCamera(float deltaTime){
    Camera& camera = renderer->camera;
    float distance = camera.movementSpeed * deltaTime;

    if(moveForward){
        camera.moveForward(distance);
    }
    if(moveBackward){
        camera.moveForward(-distance);
    }
    if(moveRight){
        camera.moveRight(distance);
    }
    if(moveLeft){
        camera.moveRight(-distance);
    }
    if(moveUp){
        camera.moveUp(distance);
    }
    if(moveDown){
        camera.moveUp(-distance);
    }
}


void QT_GLRenderer::keyPressEvent(QKeyEvent* event){
    switch(event->key()){
        case Qt::Key_W:
            moveForward = true;
            break;

        case Qt::Key_S:
            moveBackward = true;
            break;

        case Qt::Key_A:
            moveLeft = true;
            break;

        case Qt::Key_D:
            moveRight = true;
            break;

        case Qt::Key_E:
            moveUp = true;
            break;

        case Qt::Key_Q:
            moveDown = true;
            break;
    }
}

void QT_GLRenderer::keyReleaseEvent(QKeyEvent* event){
    switch(event->key()){
        case Qt::Key_W:
            moveForward = false;
            break;

        case Qt::Key_S:
            moveBackward = false;
            break;

        case Qt::Key_A:
            moveLeft = false;
            break;

        case Qt::Key_D:
            moveRight = false;
            break;

        case Qt::Key_E:
            moveUp = false;
            break;

        case Qt::Key_Q:
            moveDown = false;
            break;
    }
}

void QT_GLRenderer::mousePressEvent(QMouseEvent* event){
    if(event->button() == Qt::RightButton){
        rotating = true;
        lastMousePosition = event->position().toPoint();
        grabMouse();
    }
}

void QT_GLRenderer::mouseReleaseEvent(QMouseEvent* event){
    if(event->button() == Qt::RightButton){
        rotating = false;
        releaseMouse();
    }
}

void QT_GLRenderer::mouseMoveEvent(QMouseEvent* event){
    if(!rotating){
        return;
    }

    QPoint current = event->position().toPoint();
    QPoint delta = current - lastMousePosition;
    lastMousePosition = current;

    renderer->camera.rotate(delta.x(), -delta.y());
    update();
}

void QT_GLRenderer::wheelEvent(QWheelEvent* event){
    float steps = event->angleDelta().y() / 120.0f;

    renderer->camera.zoom(steps);
    update();
}











