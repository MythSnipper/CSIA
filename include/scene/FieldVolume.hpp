#pragma once

#include "SceneObject.hpp"


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

private:
    float GStrength = 0.0f; //Gravitational field strength, in N/kg
    float EStrength = 0.0f; //Electric field strength, in N/C

};








