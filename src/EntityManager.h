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
    Entity* canPickup(const Entity &e);
    bool isEntityInEntity(const Entity &e1, const Entity &e2);
    bool isInInventory(Entity *e, Entity *item);
    void addToInventory(Entity *e, Entity *item);
    bool isCarryable(Entity *e);
    std::vector<Entity*> getNClosestEntities(Entity *e, int n);
    int getDistanceTo(Entity *e, Entity *e2);
    int getXDistanceTo(Entity *e, Entity *e2);
    int getYDistanceTo(Entity *e, Entity *e2);
    void handleFearfulEntity(Entity *e);
    bool isWithinRange(int point, int target, int range);
    void retargetEntity(Entity *e);

};
