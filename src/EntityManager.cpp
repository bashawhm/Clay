#include "EntityManager.h"
#include "json.cpp"
#include "RenderModel.h"

#include <string>
#include <vector>

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
        Entity *e = new Entity("", -1, -1, -1, -1, None, -1, -1, -1);
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

void EntityManager::simulate() {
    for (unsigned long i = 0; i < entities.size(); ++i) {
        entities[i]->simulate(entities[entities[i]->floor]->rRect);
    }
}
