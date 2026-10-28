#pragma once

#include <QOpenGLFunctions_3_3_Core>

#include <glm/glm.hpp>

struct Transform{
    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 rotation = glm::vec3(0.0f);
    glm::vec3 scale = glm::vec3(1.0f);

    glm::mat4 get_matrix();
};