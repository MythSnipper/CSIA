#pragma once

#include <QOpenGLFunctions_3_3_Core>

#include <ShaderProgram.hpp>
#include <Texture2D.hpp>

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