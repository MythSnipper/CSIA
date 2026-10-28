#pragma once

#include "SceneObject.hpp"


class PointParticle : public SceneObject{
public:
    PointParticle(ID id, std::string name) : SceneObject(id, std::move(name)){}

    SceneObjectType getType() const override{
        return SceneObjectType::PointParticle;
    }

    float getMass(){
        return mass;
    }
    void setMass(float value){
        mass = value;
    }

    float getCharge(){
        return charge;
    }
    void setCharge(float value){
        charge = value;
    }


private:
    float mass = 1.0f;
    float charge = 0.0f;

};








