#include <GLRenderer.hpp>


const char* objectVertShaderPath = ":/shaders/object.vert";
const char* objectFragShaderPath = ":/shaders/object.frag";
const char* wireframePlaneVertShaderPath = ":/shaders/wireframeplane.vert";
const char* wireframePlaneFragShaderPath = ":/shaders/wireframeplane.frag";


void GLRenderer::initialize(){
    initializeOpenGLFunctions();

    printInfo();
    createGLResources();
}

void GLRenderer::resize(int pixelWidth, int pixelHeight){
    if(pixelWidth <= 0 || pixelHeight <= 0){
        return;
    }

    viewportWidth = pixelWidth;
    viewportHeight = pixelHeight;

    glViewport(0, 0, pixelWidth, pixelHeight);
    camera.aspect_ratio = static_cast<float>(pixelWidth) / static_cast<float>(pixelHeight);
}

void GLRenderer::render(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 view = camera.get_view_matrix();
    glm::mat4 projection = camera.get_projection_matrix();

    for(auto& object : objects){
        drawRenderObject(object.second, view, projection);
    }

    drawGrid();
}

void GLRenderer::setScene(Scene* scene){
    this->scene = scene;
};


void GLRenderer::createGLResources(){
    //make shaders
    objectShader = std::make_unique<ShaderProgram>();
    objectShader->from_files(objectVertShaderPath, objectFragShaderPath);
    wireframePlaneShader = std::make_unique<ShaderProgram>();
    wireframePlaneShader->from_files(wireframePlaneVertShaderPath, wireframePlaneFragShaderPath);

    std::vector<float> cubeVertices;
    std::vector<uint32_t> cubeIndices;
    generate_cube(1.0, cubeVertices, cubeIndices);

    std::vector<float> sphereVertices;
    std::vector<uint32_t> sphereIndices;
    generate_sphere(1.0f, 20, 20, sphereVertices, sphereIndices);

    //cube
    cube = std::make_unique<Mesh>(cubeVertices, cubeIndices);

    //sphere
    sphere = std::make_unique<Mesh>(sphereVertices.data(), sphereVertices.size() * sizeof(float), sphereIndices.data(), sphereIndices.size() * sizeof(uint32_t));

    //make textures
    uint8_t whiteColor[4] = {255, 255, 255, 255};
    whiteTexture = std::make_unique<Texture2D>(whiteColor, 1, 1, GL_RGBA, GL_RGBA);

    //64x64 checkerboard with 8px squares
    std::vector<uint8_t> pixels(64 * 64 * 4);
    for(int y = 0; y < 64; y++){
        for(int x = 0; x < 64; x++){
            uint8_t v = (((x / 8) + (y / 8)) % 2 == 0) ? 255 : 90;
            int i = (y * 64 + x) * 4;
            pixels[i + 0] = v;
            pixels[i + 1] = v;
            pixels[i + 2] = v;
            pixels[i + 3] = 255;
        }
    }
    checkerTexture = std::make_unique<Texture2D>(pixels.data(), 64, 64, GL_RGBA, GL_RGBA);



    white = createMaterial();
    white->color = glm::vec3(1.0f, 1.0f, 1.0f);

    purple = createMaterial();
    purple->color = glm::vec3(0.6f, 0.1f, 1.0f);

    checker = createMaterial();
    checker->albedo = checkerTexture.get();
    checker->tiling = glm::vec2(4.0f);

    //generate empty VAO for fullscreen shader passes
    glGenVertexArrays(1, &fullscreenVAO);

    objects.clear();

    if(scene == nullptr){
        return;
    }

    //make render objects 
    for(const auto& [id, sceneObject] : scene->getObjects()){
        switch(sceneObject->getType()){
            case SceneObjectType::PointParticle:
                createRenderObject(id, sphere.get(), purple);
            break;

            case SceneObjectType::FieldVolume:
                createRenderObject(id, cube.get(), white);
            break;
        }
        updateRenderObject(id, sceneObject->getTransform());
    }


}

void GLRenderer::syncRenderObjects(){
    if(scene == nullptr){
        objects.clear();
        return;
    }
    //add existing
    for(const auto& [id, sceneObject] : scene->getObjects()){
        auto it = objects.find(id);

        if(it == objects.end()){
            switch(sceneObject->getType()){
                case SceneObjectType::PointParticle:
                    createRenderObject(id, sphere.get(), purple);
                break;

                case SceneObjectType::FieldVolume:
                    createRenderObject(id, cube.get(), white);
                break;
            }
        }
        updateRenderObject(id, sceneObject->getTransform());
    }
    //delete stale RenderObject s if not in Scene
    for(auto it=objects.begin();it!=objects.end(); ){
        if(scene->getObject(it->first) == nullptr){
            it = objects.erase(it);
        }
        else{
            it++;
        }
    }
}

void GLRenderer::destroyGLResources(){
    objects.clear();
    materials.clear();

    cube.reset();
    sphere.reset();

    whiteTexture.reset();
    checkerTexture.reset();

    objectShader.reset();
    wireframePlaneShader.reset();

    if(fullscreenVAO != 0){
        glDeleteVertexArrays(1, &fullscreenVAO);
        fullscreenVAO = 0;
    }

}

void GLRenderer::drawRenderObject(std::unique_ptr<RenderObject>& object, const glm::mat4& view, const glm::mat4& projection){
    if(!object || !object->mesh || !object->material || !object->material->shader){
        return;
    }

    Material* material = object->material;

    material->apply();   //activates the shader first, so the sets below hit the right program

    //sets uniforms
    material->shader->set_mat4("model", false, object->transform.get_matrix());
    material->shader->set_mat4("view", false, view);
    material->shader->set_mat4("projection", false, projection);
    material->shader->set_float("cameraPosition", {camera.position.x, camera.position.y, camera.position.z});
    material->shader->set_float("lightDirection", {light.direction.x, light.direction.y, light.direction.z});
    material->shader->set_float("lightColor", {light.color.r, light.color.g, light.color.b});
    material->shader->set_float("ambientStrength", {light.ambient});

    glPolygonMode(GL_FRONT_AND_BACK, object->wireframeMode ? GL_LINE : GL_FILL);
    object->mesh->draw();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void GLRenderer::drawFullscreenPass(ShaderProgram& shader){
    shader.activate();

    glBindVertexArray(fullscreenVAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
}

void GLRenderer::drawGrid(){
    glm::mat4 view = camera.get_view_matrix();
    glm::mat4 projection = camera.get_projection_matrix();

    wireframePlaneShader->activate();

    //set shader uniforms
    wireframePlaneShader->set_mat4("inverseView", false, glm::inverse(view));
    wireframePlaneShader->set_mat4("inverseProjection", false, glm::inverse(projection));
    wireframePlaneShader->set_mat4("viewProjection", false, projection * view);

    //gl_FragCoord is in physical pixels, so scale by the device pixel ratio
    wireframePlaneShader->set_float("viewportSize", {
        (float)viewportWidth,
        (float)viewportHeight
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



void GLRenderer::createRenderObject(ID id, Mesh* mesh, Material* material){
    objects.emplace(id, std::make_unique<RenderObject>(id, mesh, material));
}
void GLRenderer::updateRenderObject(ID id, const Transform& transform){
    auto it = objects.find(id);
    if(it == objects.end()){
        return;
    }
    it->second->transform = transform;
}
void GLRenderer::destroyRenderObject(ID id){
    objects.erase(id);
}



void GLRenderer::printInfo(){
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

    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    printf("\tNumber of vertex attributes available: %d\n", nrAttributes);
    for(int i=0;i<20;i++){
        printf("-");
    }
}

Material* GLRenderer::createMaterial(){
    auto material = std::make_unique<Material>();
    material->shader = objectShader.get();
    material->albedo = whiteTexture.get();   //safe default, no null texture
    materials.push_back(std::move(material));
    return materials.back().get();
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
            float x = ringRadius * std::cos(sectorAngle);
            float z = ringRadius * std::sin(sectorAngle);

            vertices.insert(vertices.end(), {
                x, y, z,                                    //position
                x / radius, y / radius, z / radius,         //normal: position on a sphere, normalized
                static_cast<float>(j) / sectors,            //u: around the ring
                1.0f - static_cast<float>(i) / stacks       //v: 1 at the top pole, 0 at the bottom
            });
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
void generate_cube(float halfSize, std::vector<float>& vertices, std::vector<uint32_t>& indices){
    vertices.clear();
    indices.clear();

    //per face: outward normal plus the two axes spanning it (right x up = normal, so winding is CCW from outside)
    struct Face {
        glm::vec3 normal;
        glm::vec3 right;
        glm::vec3 up;
    };
    const Face faces[6] = {
        {glm::vec3( 0, 0, 1), glm::vec3( 1, 0, 0), glm::vec3(0, 1, 0)},
        {glm::vec3( 0, 0,-1), glm::vec3(-1, 0, 0), glm::vec3(0, 1, 0)},
        {glm::vec3( 1, 0, 0), glm::vec3( 0, 0,-1), glm::vec3(0, 1, 0)},
        {glm::vec3(-1, 0, 0), glm::vec3( 0, 0, 1), glm::vec3(0, 1, 0)},
        {glm::vec3( 0, 1, 0), glm::vec3( 1, 0, 0), glm::vec3(0, 0,-1)},
        {glm::vec3( 0,-1, 0), glm::vec3( 1, 0, 0), glm::vec3(0, 0, 1)}
    };
    const glm::vec2 corners[4] = {
        glm::vec2(-1, -1), glm::vec2(1, -1), glm::vec2(1, 1), glm::vec2(-1, 1)
    };

    for(const Face& face : faces){
        uint32_t first = static_cast<uint32_t>(vertices.size() / 8);

        for(const glm::vec2& corner : corners){
            glm::vec3 position = (face.normal + face.right * corner.x + face.up * corner.y) * halfSize;
            glm::vec2 uv = corner * 0.5f + 0.5f;

            vertices.insert(vertices.end(), {
                position.x, position.y, position.z,
                face.normal.x, face.normal.y, face.normal.z,
                uv.x, uv.y
            });
        }

        indices.insert(indices.end(), {
            first, first + 1, first + 2,
            first, first + 2, first + 3
        });
    }
}

