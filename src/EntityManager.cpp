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
        Entity *e = new Entity(-1, -1, -1, -1, None, -1, -1, -1);
        e->deserialize(ents[i]);
        entities.push_back(e);
    }

}

vector<RenderModel> EntityManager::getRenderModels() {
    vector<RenderModel> models;
    for (unsigned long i = 0; i < entities.size(); ++i) {
        models.push_back(entities[i]->getRenderModel());
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

void EntityManager::simulate() {
    for (unsigned long i = 0; i < entities.size(); ++i) {
        entities[i]->simulate(entities[entities[i]->floor]->rRect);
    }
}
