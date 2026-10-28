#pragma once

#include <SceneObject.hpp>
#include <PointParticle.hpp>
#include <FieldVolume.hpp>

#include <memory>
#include <unordered_map>

class Scene{
public:
    ID createParticle(std::string name){
        ID id = nextID++;
        objects.emplace(id, std::make_unique<PointParticle>(id, std::move(name)));
        return id;
    }
    ID createField(std::string name){
        ID id = nextID++;
        objects.emplace(id, std::make_unique<FieldVolume>(id, std::move(name)));
        return id;
    }

    void removeObject(ID id){
        objects.erase(id);
    }

    SceneObject* getObject(ID id){
        auto it = objects.find(id);
        if(it == objects.end()){
            return nullptr;
        }
        return it->second.get();
    }
    const SceneObject* getObject(ID id) const{
        auto it = objects.find(id);
        if(it == objects.end()){
            return nullptr;
        }
        return it->second.get();
    }
    const auto& getObjects() const{
        return objects;
    }

private:
    ID nextID = 1;
    std::unordered_map<ID, std::unique_ptr<SceneObject>> objects;
};

