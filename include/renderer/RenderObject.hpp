#pragma once

#include <QOpenGLFunctions_3_3_Core>

#include <Transform.hpp>
#include <Mesh.hpp>
#include <Material.hpp>

typedef uint32_t ID;

class RenderObject{
public:
    RenderObject(ID id, Mesh* mesh, Material* material) : id(id), mesh(mesh), material(material){}
    ID id;
    
    Transform transform;

    bool wireframeMode = false;
    Mesh* mesh = nullptr;
    Material* material = nullptr;
};