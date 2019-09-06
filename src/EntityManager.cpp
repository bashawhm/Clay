#include "EntityManager.h"
#include "json.cpp"
#include "RenderModel.h"

#include <string>
#include <vector>
#include <math.h>

using namespace std;
using json = nlohmann::json;

string EntityManager::serialize() {
    vector<string> ents;
    for (unsigned long i = 0; i < entities.size(); ++i) {
        ents.push_back(entities[i]->serialize());
    }

    json j = {
        {"entities", ents},
    };
    return j.dump();
}

void EntityManager::deserialize(string in) {
    json j;
    j = json::parse(in);

    vector<string> ents = j["entities"];
    for (unsigned long i = 0; i < ents.size(); ++i) {
        Entity *e = new Entity("", -1, -1, -1, -1, None, -1, -1, -1, -1);
        e->deserialize(ents[i]);
        entities.push_back(e);
    }

}

vector<RenderModel> EntityManager::getRenderModels() {
    vector<RenderModel> models;
    for (unsigned long i = 0; i < entities.size(); ++i) {
        if (!(entities[i]->inInventory)) {
            models.push_back(entities[i]->getRenderModel());
        }
    }
    return models;
}

Entity* EntityManager::getFollowing() {
    for (unsigned long i = 0; i < entities.size(); ++i) {
        if (entities[i]->following) {
            return entities[i];
        }
    }
    return nullptr;
}

bool EntityManager::isEntityInEntity(const Entity &e1, const Entity &e2) {
    if (e2.rRect.y <= (e1.rRect.y + e1.rRect.h) && (e2.rRect.y + e2.rRect.h) >= e1.rRect.y && e2.rRect.x <= (e1.rRect.x + e1.rRect.w) && (e2.rRect.x + e2.rRect.w) >= e1.rRect.x) {
        return true;
    }
    return false;
}

bool EntityManager::isCarryable(Entity *e) {
    for (unsigned long i = 0; i < e->attributes.size(); ++i) {
        if (e->attributes[i] == Carryable) {
            return true;
        }
    }
    return false;
}

Entity* EntityManager::canPickup(const Entity &e) {
    for (unsigned long i = 0; i < entities.size(); ++i) {
        if (!isCarryable(entities[i])) {
            continue;
        }
        
        if (&e == entities[i]) {
            continue;
        }
        if (!entities[i]->inInventory) {
            if (isEntityInEntity(e, *entities[i])) {
                return entities[i];
            }
        }
    }
    return nullptr;
}

bool EntityManager::isInInventory(Entity *e, Entity *item) {
    for (unsigned long i = 0; i < e->inventory.size(); ++i) {
        if (e->inventory[i] == item) {
            return true;
        }
    }
    return false;
}

void EntityManager::addToInventory(Entity *e, Entity *item) {
    if (isInInventory(e, item)) {
        return;
    }

    e->inventory.push_back(item);
    item->inInventory = true;
}

void EntityManager::retargetEntity(Entity *e) {
    e->targetX = (rand() % entities[(e->floor)]->rRect.w) + entities[(e->floor)]->rRect.x;
    e->targetY = (rand() % entities[(e->floor)]->rRect.h) + entities[(e->floor)]->rRect.y;
}

int EntityManager::getXDistanceTo(Entity *e, Entity *e2) {
    int origin1x = (e->rRect.x + e->rRect.w) / 2;
    int origin2x = (e2->rRect.x + e2->rRect.w) / 2;

    int dx = origin2x - origin1x;

    return dx;
}

int EntityManager::getYDistanceTo(Entity *e, Entity *e2) {
    int origin1y = (e->rRect.y + e->rRect.h) / 2;
    int origin2y = (e2->rRect.y + e2->rRect.h) / 2;

    int dy = origin2y - origin1y;

    return dy;
}

int EntityManager::getDistanceTo(Entity *e, Entity *e2) {
    int dx = getXDistanceTo(e, e2);
    int dy = getYDistanceTo(e, e2);

    return sqrt((dx * dx) + (dy * dy));
}


bool entSort(pair<int, Entity *> e1, pair<int, Entity *> e2) {
    return (e1.first < e2.first);
}

vector<Entity*> EntityManager::getNClosestEntities(Entity *e, int n) {
    vector<pair<int, Entity*>> distance;
    for (unsigned long i = 0; i < entities.size(); ++i) {
        if (entities[i] == e) {
            continue;
        }
        pair<int, Entity *> e1;
        e1.first = getDistanceTo(e, entities[i]);
        e1.second = entities[i];
        distance.push_back(e1);
    }
    sort(distance.begin(), distance.end(), entSort);
    vector<Entity*> ents;
    for (int i = 0; i < n; ++i) {
        ents.push_back(distance[i].second);
    }

    return ents;
}

void EntityManager::handleFearfulEntity(Entity *e) {
    vector<Entity *> ents = getNClosestEntities(e, 5);
    vector<Entity *> entsInRange;
    for (unsigned long i = 0; i < ents.size(); ++i) {
        int dis = getDistanceTo(e, ents[i]);
        if (ents[i]->intimidation - dis < e->resolve) {
            continue;
        }
        entsInRange.push_back(ents[i]);
    }
     if (entsInRange.size() == 0) {
        return;
    }

    int aveX = 0;
    int aveY = 0;
    for (unsigned long i = 0; i < entsInRange.size(); i++) {
        aveX += getXDistanceTo(e, entsInRange[i]);
        aveY += getYDistanceTo(e, entsInRange[i]);
    }
    aveX /= entsInRange.size();
    aveY /= entsInRange.size();

    int aggX = -1;
    int aggY = -1;
    if (aveX < 0) {
        aggX *= -1;
    }
    if (aveY < 0) {
        aggY *= -1;
    }

    retargetEntity(e);
    e->moveX(entities[e->floor]->rRect, e->moveSpeed * aggX);
    e->moveY(entities[e->floor]->rRect, e->moveSpeed * aggY);
}

bool EntityManager::isWithinRange(int point, int target, int range) {
    if (point == target)
        return true;

    if (point < target && point > target-range) {
        return true;
    }
    if (point > target && point < target+range) {
        return true;
    }

    return false;
}

void EntityManager::simulate() {
    for (unsigned long i = 0; i < entities.size(); ++i) {
        if (entities[i]->hasAttribute(NilSimulation)) {
            continue;
        }
        entities[i]->simulate(entities[entities[i]->floor]->rRect);
        if (entities[i]->hasAttribute(Fearful)) {
            handleFearfulEntity(entities[i]);
        }
        if (entities[i]->hasAttribute(AutoWandering)) {
            if (isWithinRange(entities[i]->rRect.x, entities[i]->targetX, entities[i]->moveSpeed) && isWithinRange(entities[i]->rRect.y, entities[i]->targetY, entities[i]->moveSpeed)) {
                retargetEntity(entities[i]);
            }
        }
    }
}
