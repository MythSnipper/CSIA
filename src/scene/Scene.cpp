#include <Scene.hpp>

ID Scene::addObject(std::unique_ptr<SceneObject> object){
    objects.insert({nextID, object});
    nextID++;
    return nextID-1;
}

void Scene::removeObject(ID id){
    objects.erase(id);
}

SceneObject* Scene::getObject(ID id){
    return objects.at(id).get();
}

const SceneObject* Scene::getObject(ID id) const{
    return objects.at(id).get();
}

