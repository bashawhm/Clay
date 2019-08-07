#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cstring>

#include "SDL2/SDL2_framerate.h"
#include "SDL2/SDL_messagebox.h"

#include "Renderer.h"
#include "RenderModel.h"
#include "test.h"
#include "EntityManager.h"
#include "Stage.h"

#define SCALE_FACTOR 10

void showHelp() {
    SDL_MessageBoxButtonData buttons = {SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 0, "Close"};
    string help;
    help += "Press h to see this menu\n";
    help += "Press w,a,s,d to move\n";
    help += "Press Space to pickup an item\n";
    help += "Press e to show your inventory\n";
    help += "Press i to show your status info\n";
    SDL_MessageBoxData message = {SDL_MESSAGEBOX_INFORMATION, NULL, "Help", help.c_str(), 1, &buttons, NULL};
    int butId;
    SDL_ShowMessageBox(&message, &butId);
}

void showEntityStatus(Entity *e) {
    SDL_MessageBoxButtonData buttons = {SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 0, "Close"};
    string status;
    status += "Name: ";
        status += e->name;
        status += "\n";
    status += "Health: ";
        status += to_string(e->health);
        status += "\n";
    status += "Resolve: ";
        status += to_string(e->resolve);
        status += "\n";
    status += "Move Speed: ";
        status += to_string(e->moveSpeed);
        status += "\n";
    SDL_MessageBoxData message = {SDL_MESSAGEBOX_INFORMATION, NULL, "Status", status.c_str(), 1, &buttons, NULL};
    int butId;
    SDL_ShowMessageBox(&message, &butId);

}

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
            int floorWidth = 1920 * SCALE_FACTOR;    //Arbitrary sizes based on the pixel width of the original image x10
            int floorHeight = 1080 * SCALE_FACTOR;
            Entity *floor = new Entity("Floor", 0, 0, floorWidth, floorHeight, Grass, 10000000, 0, 0, 0);
            Entity *player = new Entity("Player", 500, 500, 10 * SCALE_FACTOR, 10 * SCALE_FACTOR, Player, 50, 10, 1, 10);
            player->following = true;
            stage.eManager.entities.push_back(floor);
            stage.eManager.entities.push_back(player);

            int numGrass = rand() % 300;
            for (int i = 0; i < numGrass; ++i) {
                Entity *tGrass = new Entity("Grass", rand()%(floorWidth-32), rand()%(floorHeight-32), 32 * (SCALE_FACTOR / 2), 32 * (SCALE_FACTOR / 2), TallGrass, 1, 0, 0, 1);
                tGrass->attributes.push_back(AutoWandering);
                tGrass->attributes.push_back(Carryable);
                stage.eManager.entities.push_back(tGrass);
            }

            int numBirds = rand() % 50;
            for (int i = 0; i < numBirds; ++i) {
                Entity *bird = new Entity("Bird", rand()%(floorWidth-32), rand()%(floorHeight-32), 32 * (SCALE_FACTOR / 5), 32 * (SCALE_FACTOR / 5), Bird, 5, 5, 0, 50);
                bird->attributes.push_back(AutoWandering);
                bird->attributes.push_back(Carryable);
                bird->attributes.push_back(Fearful);
                stage.eManager.entities.push_back(bird);
            }

            int numCats = rand() % 50;
            for (int i = 0; i < numCats; ++i) {
                Entity *cat = new Entity("Cat", rand()%(floorWidth-32), rand()%(floorHeight-32), 32 * (SCALE_FACTOR / 5), 32 * (SCALE_FACTOR / 5), Cat, 15, 5, 0, 20);
                cat->attributes.push_back(AutoWandering);
                cat->attributes.push_back(Carryable);
                cat->attributes.push_back(Fearful);
                stage.eManager.entities.push_back(cat);
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
                    SDL_Rect bounds = stage.eManager.entities[player->floor]->rRect;
                    player->moveY(bounds, -player->moveSpeed);
                    break;
                }
                case SDLK_a: {
                    Entity *player = stage.eManager.getFollowing();
                    SDL_Rect bounds = stage.eManager.entities[player->floor]->rRect;
                    player->moveX(bounds, -player->moveSpeed);
                    break;
                }
                case SDLK_s: {
                    Entity *player = stage.eManager.getFollowing();
                    SDL_Rect bounds = stage.eManager.entities[player->floor]->rRect;
                    player->moveY(bounds, player->moveSpeed);
                    break;
                }
                case SDLK_d: {
                    Entity *player = stage.eManager.getFollowing();
                    SDL_Rect bounds = stage.eManager.entities[player->floor]->rRect;
                    player->moveX(bounds, player->moveSpeed);
                    break;
                }
                case SDLK_SPACE: {
                    Entity *player = stage.eManager.getFollowing();
                    for (unsigned long i = 0; i < stage.eManager.entities.size(); ++i) {
                        if (stage.eManager.entities[i]->floor == i) {
                            continue;
                        }
                        if (stage.eManager.isEntityInEntity(*player, *(stage.eManager.entities[i]))) {
                            Entity *item = stage.eManager.canPickup(*(stage.eManager.entities[i]));
                            if (item != nullptr) {
                                stage.eManager.addToInventory(player, item);
                                break;
                            }
                        }
                    }
                    break;
                }
                case SDLK_e: {
                    Entity *player = stage.eManager.getFollowing();
                    SDL_MessageBoxButtonData buttons = {SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 0, "Close"};
                    string inv;
                    for (unsigned long i = 0; i < player->inventory.size(); ++i) {
                        inv += player->inventory[i]->name + "\n";
                    }
                    SDL_MessageBoxData message = {SDL_MESSAGEBOX_INFORMATION, NULL, "Inventory", inv.c_str(), 1, &buttons, NULL};
                    int butId;
                    SDL_ShowMessageBox(&message, &butId);
                    break;
                }
                case SDLK_i: {
                    Entity *player = stage.eManager.getFollowing();
                    showEntityStatus(player);
                    break;
                }
                case SDLK_h: {
                    showHelp();
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
