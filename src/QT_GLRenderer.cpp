#include "QT_GLRenderer.hpp"

#include <cstdio>

const char* objectVertShaderPath = ":/shaders/object.vert";
const char* objectFragShaderPath = ":/shaders/object.frag";
const char* wireframePlaneVertShaderPath = ":/shaders/wireframeplane.vert";
const char* wireframePlaneFragShaderPath = ":/shaders/wireframeplane.frag";

QT_GLRenderer::QT_GLRenderer(QWidget* parent) : QOpenGLWidget(parent){
    //settings
    wireframe_mode = true;
    depth_test = true;
    FPS = 60;

    setFocusPolicy(Qt::StrongFocus);

    connect(&renderTimer, &QTimer::timeout, this, [this](){
            float deltaTime = frameTimer.restart() / 1000.0f;
            updateCamera(deltaTime);
            update();
        }
    );

    //renderTimer configuration
    renderTimer.setInterval(1000 / FPS); //how much ms to delay to achieve the target fps
    frameTimer.start();
    renderTimer.start();
}

QT_GLRenderer::~QT_GLRenderer(){
    makeCurrent();

    doneCurrent();
}

void QT_GLRenderer::initializeGL(){
    initializeOpenGLFunctions();

    connect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &QT_GLRenderer::destroyGLResources, Qt::DirectConnection);

    printInfo();
    applySettings();

    createGLResources();

    update();
}

void QT_GLRenderer::resizeGL(int width, int height){
    glViewport(0, 0, width, height);

    camera.aspect_ratio = ((float)width) / ((float)height);
}

void QT_GLRenderer::paintGL(){
    //clear screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //draw render objects
    for(auto& object : objects){
        drawRenderObject(object);
    }

    drawGrid();

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

    camera.rotate(delta.x(), -delta.y());
    update();
}

void QT_GLRenderer::wheelEvent(QWheelEvent* event){
    float steps = event->angleDelta().y() / 120.0f;

    camera.zoom(steps);
    update();
}

void QT_GLRenderer::updateCamera(float deltaTime){
    if(moveForward){
        camera.moveForward(camera.movementSpeed * deltaTime);
    }
    if(moveBackward){
        camera.moveForward(-camera.movementSpeed * deltaTime);
    }
    if(moveRight){
        camera.moveRight(camera.movementSpeed * deltaTime);
    }
    if(moveLeft){
        camera.moveRight(-camera.movementSpeed * deltaTime);
    }
    if(moveUp){
        camera.moveUp(camera.movementSpeed * deltaTime);
    }
    if(moveDown){
        camera.moveUp(-camera.movementSpeed * deltaTime);
    }
}

void QT_GLRenderer::createGLResources(){
    //make shader
    objectShader = std::make_unique<ShaderProgram>();
    objectShader->from_files(objectVertShaderPath, objectFragShaderPath);
    wireframePlaneShader = std::make_unique<ShaderProgram>();
    wireframePlaneShader->from_files(wireframePlaneVertShaderPath, wireframePlaneFragShaderPath);

    //vertices defining a cube
    float cubeVertices[] = {
        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };
    uint32_t cubeIndices[] = {
        0, 1, 2,
        1, 2, 3,
        4, 0, 6,
        0, 6, 2,
        5, 1, 7,
        1, 7, 3,
        4, 5, 6,
        5, 6, 7,
        0, 1, 4,
        1, 4, 5,
        2, 3, 6,
        3, 6, 7,
    };

    std::vector<float> sphereVertices;
    std::vector<uint32_t> sphereIndices;


    //cube
    mesh = std::make_unique<Mesh>(cubeVertices, sizeof(cubeVertices), cubeIndices, sizeof(cubeIndices));

    //sphere
    sphere = std::make_unique<Mesh>(sphereVertices, sizeof(sphereVertices), sphereIndices, sizeof(sphereIndices));

    objects.clear();

    //make render objects
    auto cube1 = std::make_unique<RenderObject>();
    cube1->transform.position = glm::vec3(-2.0f, 0.0f, 0.0f);
    cube1->mesh = mesh.get();
    cube1->shader = objectShader.get();
    objects.push_back(std::move(cube1));

    auto cube2 = std::make_unique<RenderObject>();
    cube2->transform.position = glm::vec3(2.0f, 0.0f, 0.0f);
    cube2->mesh = mesh.get();
    cube2->shader = objectShader.get();
    objects.push_back(std::move(cube2));

    auto cube3 = std::make_unique<RenderObject>();
    cube3->transform.position = glm::vec3(0.0f, 0.0f, 0.0f);
    cube3->transform.scale = glm::vec3(0.4f);
    cube3->mesh = mesh.get();
    cube3->shader = objectShader.get();
    objects.push_back(std::move(cube3));

    auto cube4 = std::make_unique<RenderObject>();
    cube4->transform.position = glm::vec3(0.0f, -3.0f, -5.0f);
    cube4->transform.scale = glm::vec3(0.5f, 2.0f, 0.5f);
    cube4->mesh = mesh.get();
    cube4->shader = objectShader.get();
    objects.push_back(std::move(cube4));

    auto sphere1 = std::make_unique<RenderObject>();
    sphere1->transform.position = glm::vec3(0.0f, 4.0f, 1.0f);
    sphere1->transform.scale = glm::vec3(1.0f, 10f, 1.0f);
    sphere1->mesh = sphere.get();
    sphere1->shader = objectShader.get();
    objects.push_back(std::move(sphere1));

    //generate empty VAO for fullscreen shader passes
    glGenVertexArrays(1, &fullscreenVAO);

}

void QT_GLRenderer::destroyGLResources(){
    makeCurrent();

    objects.clear();

    mesh.reset();
    objectShader.reset();
    wireframePlaneShader.reset();

    if(fullscreenVAO != 0){
        glDeleteVertexArrays(1, &fullscreenVAO);
        fullscreenVAO = 0;
    }

    doneCurrent();
}



void QT_GLRenderer::drawRenderObject(std::unique_ptr<RenderObject>& object){
    glm::mat4 model = object->transform.get_matrix();
    glm::mat4 view = camera.get_view_matrix();
    glm::mat4 projection = camera.get_projection_matrix();

    object->shader->activate();

    //needed by object shader
    object->shader->set_mat4("model", false, model);
    object->shader->set_mat4("view", false, view);
    object->shader->set_mat4("projection", false, projection);

    //needed by wireframeplane shader
    object->shader->set_mat4("inverseView", false, glm::inverse(view));
    object->shader->set_mat4("inverseProjection", false, glm::inverse(projection));
    object->shader->set_mat4("viewProjection", false, glm::mat4(projection * view));
    object->shader->set_float("viewportSize", {(float)width(), (float)height()});
    object->shader->set_float("spacing", {1.0f});
    object->shader->set_float("cameraPosition", {camera.position.x, camera.position.y, camera.position.z});

    if(object->wireframeMode){
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    object->mesh->draw();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void QT_GLRenderer::drawFullscreenPass(ShaderProgram& shader){
    shader.activate();

    glBindVertexArray(fullscreenVAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
}

void QT_GLRenderer::drawGrid(){
    glm::mat4 view = camera.get_view_matrix();
    glm::mat4 projection = camera.get_projection_matrix();

    wireframePlaneShader->activate();

    //set shader uniforms
    wireframePlaneShader->set_mat4("inverseView", false, glm::inverse(view));
    wireframePlaneShader->set_mat4("inverseProjection", false, glm::inverse(projection));
    wireframePlaneShader->set_mat4("viewProjection", false, projection * view);

    //gl_FragCoord is in physical pixels, so scale by the device pixel ratio
    float ratio = static_cast<float>(devicePixelRatioF());
    wireframePlaneShader->set_float("viewportSize", {
        width() * ratio,
        height() * ratio
    });

    wireframePlaneShader->set_float("spacing", {1.0f});
    wireframePlaneShader->set_float("cameraPosition", {
        camera.position.x,
        camera.position.y,
        camera.position.z
    });

    //depth test against the scene, but don't overwrite its depth values
    glDepthMask(GL_FALSE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    drawFullscreenPass(*wireframePlaneShader);

    glDisable(GL_BLEND);
    glDepthMask(GL_TRUE);
}

void QT_GLRenderer::printInfo(){
    for(int i=0;i<20;i++){
        printf("-");
    }
    puts("");
    puts("Info:");
    printf("\tOS: ");
    #ifdef _WIN32
        puts("Windows");
    #else
        puts("Linux");
    #endif
    printf("\tWireframe mode: %s\n", (wireframe_mode) ? "On" : "Off");
    printf("\tDepth testing: %s\n", (depth_test) ? "On" : "Off");

    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    printf("\tNumber of vertex attributes available: %d\n", nrAttributes);
    for(int i=0;i<20;i++){
        printf("-");
    }
}

void QT_GLRenderer::applySettings(){
    glPolygonMode(GL_FRONT_AND_BACK, (wireframe_mode) ? GL_LINE : GL_FILL);
    if(depth_test){
        glEnable(GL_DEPTH_TEST);
    }
    else{
        glDisable(GL_DEPTH_TEST);
    }
}



void generate_sphere(float radius, uint32_t sectors, uint32_t stacks, std::vector<float>& vertices, std::vector<uint32_t>& indices){
    vertices.clear();
    indices.clear();

    const float pi = glm::pi<float>();

    //one ring of vertices per stack, from the top pole (+y) to the bottom pole (-y)
    for(uint32_t i = 0; i <= stacks; i++){
        float stackAngle = pi / 2.0f - i * pi / stacks; //phi
        float ringRadius = radius * std::cos(stackAngle); //r cos(phi)
        float y = radius * std::sin(stackAngle); //(r cos(phi))sin(theta)

        //sectors + 1 vertices per ring, the last one duplicates the first to close the seam
        for(uint32_t j = 0; j <= sectors; j++){
            float sectorAngle = j * 2.0f * pi / sectors;

            vertices.push_back(ringRadius * std::cos(sectorAngle));
            vertices.push_back(y);
            vertices.push_back(ringRadius * std::sin(sectorAngle));
        }
    }

    //two triangles per quad between neighbouring rings
    for(uint32_t i = 0; i < stacks; i++){
        uint32_t top = i * (sectors + 1);
        uint32_t bottom = top + sectors + 1;

        for(uint32_t j = 0; j < sectors; j++, top++, bottom++){
            //the top ring at the pole collapses to a point, so its first triangle is degenerate
            if(i != 0){
                indices.push_back(top);
                indices.push_back(top + 1);
                indices.push_back(bottom);
            }
            //same for the bottom pole
            if(i != stacks - 1){
                indices.push_back(top + 1);
                indices.push_back(bottom + 1);
                indices.push_back(bottom);
            }
        }
    }
}















