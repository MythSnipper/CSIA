#pragma once

#include <Transform.hpp>

typedef uint32_t ID;

enum class SceneObjectType{
    PointParticle,
    FieldVolume
};

class SceneObject{
public:

    SceneObject(ID id, std::string name) : id(id), name(std::move(name)){}
    virtual ~SceneObject() = default;

    ID getID(){
        return id;
    }

    const std::string& getName(){
        return name;
    }
    void setName(std::string newName){
        if(!newName.empty()){
            name = std::move(newName);
        }
    }

    const Transform& getTransform() const{
        return transform;
    }

    void setPosition(const glm::vec3& position){
        transform.position = position;
    }
    void setRotation(const glm::vec3& rotation){
        transform.rotation = rotation;
    }
    void setScale(const glm::vec3& scale){
        transform.scale = scale;
    }

    virtual SceneObjectType getType() const = 0;

private:
    ID id;
    std::string name;
    Transform transform;

};
