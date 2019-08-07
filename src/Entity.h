#pragma once

#include <string>
#include <vector>

#include "SDL2/SDL.h"

#include "Texture.h"
#include "RenderModel.h"

enum EntityAttributes {
    AutoWandering,
    Carryable,
    Fearful,
};

class Entity {
public:
    Entity(std::string eName, int rX, int rY, int w, int h, TextureIdx t, int hth, int res, int intim, int mvS);
    std::string serialize();
    void deserialize(std::string in);

    RenderModel getRenderModel();

    SDL_Rect rRect; //Real rect, not display rect, used for simulation only
    SDL_Rect dRect; //Display rect, not the real rect, used for display only
    int floor;
    bool following;
    bool inInventory;
    TextureIdx tex;
    std::string name;

    int health;
    int resolve;
    int intimidation;
    int moveSpeed;
    std::vector<Entity*> inventory;
    std::vector<EntityAttributes> attributes;

    void simulate(SDL_Rect bounds);
    void moveX(SDL_Rect bounds, int moveBy);
    void moveY(SDL_Rect bounds, int moveBy);
    bool hasAttribute(EntityAttributes att);
    

    bool operator ==(const Entity &e) const {
        if (rRect.x != e.rRect.x) {
            return false;
        } else if (rRect.y != e.rRect.y) {
            return false;
        } else if (rRect.w != e.rRect.w) {
            return false;
        } else if (rRect.h != e.rRect.h) {
            return false;
        } else if (dRect.x != e.dRect.x) {
            return false;
        } else if (dRect.y != e.dRect.y) {
            return false;
        } else if (dRect.w != e.dRect.w) {
            return false;
        } else if (dRect.h != e.dRect.h) {
            return false;
        } else if (floor != e.floor) {
            return false;
        } else if (following != e.following) {
            return false;
        } else if (inInventory != e.inInventory) {
            return false;
        } else if (tex != e.tex) {
            return false;
        } else if (name != e.name) {
            return false;
        } else if (health != e.health) {
            return false;
        } else if (resolve != e.resolve) {
            return false;
        } else if (intimidation != e.intimidation) {
            return false;
        } else if (moveSpeed != e.moveSpeed) {
            return false;
        } 
        if (inventory.size() != e.inventory.size()) {
            return false;
        } else if (attributes.size() != e.attributes.size()) {
            return false;
        }
        for (unsigned long i = 0; i < inventory.size(); ++i) {
            if (*(inventory[i]) != *(e.inventory[i])) {
                return false;
            }
        }
        for (unsigned long i = 0; i < attributes.size(); ++i) {
            if (attributes[i] != e.attributes[i]) {
                return false;
            }
        }
        return true;
    }

    bool operator !=(const Entity & e) const {
        return !(*this == e);
    }
};
