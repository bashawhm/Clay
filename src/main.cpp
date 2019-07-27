#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cstring>

#include "SDL2/SDL2_framerate.h"

#include "Renderer.h"
#include "RenderModel.h"
#include "test.h"
#include "EntityManager.h"
#include "Stage.h"

int main(int argc, char **argv) {
    SDL_Init(SDL_INIT_EVERYTHING);
    srand(time(NULL));
    if (argc > 1) {
        if (strcmp(argv[1], "--test") == 0) {
            bool correct = runAllTests(5);
            if (correct) {
                return 1;
            }
            return -1;
        } else if (strcmp(argv[1], "--gen") == 0) {
            Stage stage;
            int floorWidth = 1920;
            int floorHeight = 1080;
            Entity *floor = new Entity(0, 0, floorWidth, floorHeight, Grass, 10000000, 0, 0);
            Entity *e = new Entity(50, 50, 10, 10, Dot, 50, 10, 10);
            e->following = true;
            stage.eManager.entities.push_back(floor);
            stage.eManager.entities.push_back(e);

            int numGrass = rand() % 50;
            for (int i = 0; i < numGrass; ++i) {
                Entity *tGrass = new Entity(rand()%(floorWidth-32), rand()%(floorHeight-32), 32, 32, TallGrass, 1, 0, 1);
                tGrass->attributes.push_back(AutoWandering);
                stage.eManager.entities.push_back(tGrass);
            }

            ofstream fout("base.json");
            fout << stage.serialize() << endl;
            fout.close();
            return 1;
        }
    }
    FPSmanager fpsManager;
    SDL_initFramerate(&fpsManager);
    SDL_setFramerate(&fpsManager, 60);
    Stage stage;
    //Handle inporting a save file
    ifstream fin;
    fin.open("base.json");
    if (fin.fail()) {
        cerr << "Failed to open save template base.json" << endl;
        exit(-1);
    }
    if (argc > 1) {
        fin.open(argv[1]);
        if (fin.fail()) {
            cerr << "Failed to open save file: " << argv[1] << endl;
            exit(-1);
        }
    }
    stringstream sin;
    sin << fin.rdbuf(); 
    stage.deserialize(sin.str());

    //Main game loop
    while(stage.running) {
        SDL_Event event;
        SDL_PollEvent(&event);
        switch (event.type) {
        case SDL_QUIT: {
            //Save game and quit
            string tmp = stage.serialize();
            ofstream fout("save.json");
            fout << tmp;
            stage.running = false;
            break;
        }
        case SDL_KEYDOWN: {
            switch (event.key.keysym.sym) {
                case SDLK_w: {
                    Entity *player = stage.eManager.getFollowing();
                    player->rRect.y -= player->moveSpeed;
                    break;
                }
                case SDLK_a: {
                    Entity *player = stage.eManager.getFollowing();
                    player->rRect.x -= player->moveSpeed;
                    break;
                }
                case SDLK_s: {
                    Entity *player = stage.eManager.getFollowing();
                    player->rRect.y += player->moveSpeed;
                    break;
                }
                case SDLK_d: {
                    Entity *player = stage.eManager.getFollowing();
                    player->rRect.x += player->moveSpeed;
                    break;
                }

            }
            break;
        }
        }

        stage.simulate();
        stage.rend.renderables = stage.eManager.getRenderModels();
        stage.rend.renderDraw();
        SDL_framerateDelay(&fpsManager);
    }

    return 0;
}