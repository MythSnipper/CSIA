#pragma once

#include <QOpenGLFunctions_3_3_Core>

#include <Transform.hpp>
#include <Mesh.hpp>
#include <Material.hpp>

class RenderObject{
public:
    Transform transform;

    bool wireframeMode = false;
    Mesh* mesh = nullptr;
    Material* material = nullptr;
};