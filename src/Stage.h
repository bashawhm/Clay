#pragma once

#include "Renderer.h"
#include "EntityManager.h"

#include <string>

class Stage {
public:
    Stage();

    Renderer rend;
    EntityManager eManager;
    bool running;

    std::string serialize();
    void deserialize(std::string in);

    void simulate();
};
