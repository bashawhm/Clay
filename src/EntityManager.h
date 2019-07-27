#pragma once
#include "Entity.h"                       
#include "RenderModel.h"

#include <vector>
#include <string>

class EntityManager {
public:
    std::vector<Entity*> entities;

    std::string serialize();
    void deserialize(std::string in);
    std::vector<RenderModel> getRenderModels();
    Entity* getFollowing();

    void simulate();

};
