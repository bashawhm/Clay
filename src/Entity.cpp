#include "Entity.h"

#include <string>

#include "Texture.h"
#include "RenderModel.h"
#include "json.cpp"
#include "SDL2/SDL.h"

using json = nlohmann::json;

using namespace std;

Entity::Entity(string eName, int rX, int rY, int w, int h, TextureIdx t, int hth, int res, int mvS) {
    name = eName;
    rRect.x = rX;
    rRect.y = rY;
    rRect.w = w;
    rRect.h = h;

    dRect.x = 0;
    dRect.y = 0;
    dRect.w = w;
    dRect.h = h;

    floor = 0;
    following = false;
    inInventory = false;
    tex = t;
    health = hth;
    resolve = res;
    moveSpeed = mvS;
}

RenderModel Entity::getRenderModel() {
    RenderModel m(rRect, dRect, following, tex, moveSpeed);
    return m;
}

void Entity::moveX(int moveBy) {
    rRect.x += moveBy;
    for (unsigned long i = 0; i < inventory.size(); ++i) {
        inventory[i]->moveX(moveBy);
    }
}

void Entity::moveY(int moveBy) {
    rRect.y += moveBy;
    for (unsigned long i = 0; i < inventory.size(); ++i) {
        inventory[i]->moveY(moveBy);
    }

}

string Entity::serialize() {
    json j = {
        {"name", name},
        {"rRect", {rRect.x, rRect.y, rRect.w, rRect.h}},
        {"dRect", {dRect.x, dRect.y, dRect.w, dRect.h}},
        {"floor", floor},
        {"following", following},
        {"inInventory", inInventory},
        {"tex", tex},
        {"health", health},
        {"resolve", resolve},
        {"moveSpeed", moveSpeed},
        {"attributes", attributes},
    };
    vector<string> inv;
    for (unsigned long i = 0; i < inventory.size(); ++i) {
        inv.push_back(inventory[i]->serialize());
    }
    j["inventory"] = inv;

    return j.dump();

}

void Entity::deserialize(string in) {
    json j;
    j = json::parse(in);
    inventory.clear();
    attributes.clear();
    
    name = j["name"];
    rRect.x = j["rRect"][0];
    rRect.y = j["rRect"][1];
    rRect.w = j["rRect"][2];
    rRect.h = j["rRect"][3];

    dRect.x = j["dRect"][0];
    dRect.y = j["dRect"][1];
    dRect.w = j["dRect"][2];
    dRect.h = j["dRect"][3];

    floor = j["floor"];
    following = j["following"];
    inInventory = j["inInventory"];
    tex = j["tex"];
    health = j["health"];
    resolve = j["resolve"];
    moveSpeed = j["moveSpeed"];
    vector<EntityAttributes> att = j["attributes"];
    attributes = att;
    vector<string> inv = j["inventory"];
    for (unsigned long i = 0; i < inv.size(); ++i) {
        Entity *e = new Entity("", 0, 0, 0, 0, None, 0, 0, 0);
        e->deserialize(inv[i]);
        inventory.push_back(e);
    }

}

void Entity::simulate(SDL_Rect bounds) {
    for (unsigned long i = 0; i < attributes.size(); ++i) {
        switch(attributes[i]) {
            case AutoWandering: {
                int dxp = 0;
                int dyp = 0;
                int dxn = 0;
                int dyn = 0;
                if (rRect.x < (bounds.x+bounds.w)) {
                    dxp = rand() % moveSpeed;
                }
                if (rRect.y < (bounds.y+bounds.h)) {
                    dyp = rand() % moveSpeed;
                }
                if (rRect.x > bounds.x) {
                    dxn = -1*rand() % moveSpeed;
                }
                if (rRect.y > bounds.y) {
                    dyn = -1*rand() % moveSpeed;
                }

                rRect.x += (dxp + dxn);
                rRect.y += (dyp + dyn);
                break;
            }
        }
    }
}
