#pragma once

#include <SceneObject.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

class FieldVolume : public SceneObject{
public:
    FieldVolume(ID id, std::string name) : SceneObject(id, std::move(name)){}

    SceneObjectType getType() const override{
        return SceneObjectType::FieldVolume;
    }

    float getGStrength(){
        return GStrength;
    }
    void setGStrength(float value){
        GStrength = value;
    }

    float getEStrength(){
        return EStrength;
    }
    void setEStrength(float value){
        EStrength = value;
    }

    //get the direction vector based on the field's rotation, representing the orientation
    glm::vec3 getDirection(){
        glm::quat rotation = glm::quat(glm::radians(getTransform().rotation));
    
        return glm::normalize(rotation * glm::vec3(0.0f, 1.0f, 0.0f));
    }

private:
    float GStrength = 0.0f; //Gravitational field strength, in N/kg
    float EStrength = 0.0f; //Electric field strength, in N/C

};








