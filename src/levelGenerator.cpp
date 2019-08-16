#include "levelGenerator.h"

#include "Stage.h"
#include "Entity.h"

#define SCALE_FACTOR 10

Stage *baseGenerator() {
    Stage *stage = new Stage;
    int floorWidth = 1920 * SCALE_FACTOR;    //Arbitrary sizes based on the pixel width of the original image x10
    int floorHeight = 1080 * SCALE_FACTOR;
    Entity *floor = new Entity("Floor", 0, 0, floorWidth, floorHeight, Grass, 10000000, 0, 0, 0);
    Entity *player = new Entity("Player", 500, 500, 10 * SCALE_FACTOR, 10 * SCALE_FACTOR, Player, 50, 10, 10, 10);
    player->following = true;
    stage->eManager.entities.push_back(floor);
    stage->eManager.entities.push_back(player);

    int numGrass = rand() % 300;
    for (int i = 0; i < numGrass; ++i) {
        Entity *tGrass = new Entity("Grass", rand()%(floorWidth-(32 * (SCALE_FACTOR / 2))), rand()%(floorHeight-(32 * (SCALE_FACTOR / 2))), 16 * (SCALE_FACTOR / 2), 16 * (SCALE_FACTOR / 2), TallGrass, 1, 0, 0, 0);
        tGrass->attributes.push_back(AutoWandering);
        tGrass->attributes.push_back(Carryable);
        stage->eManager.entities.push_back(tGrass);
    }

    int numBirds = rand() % 50;
    for (int i = 0; i < numBirds; ++i) {
        Entity *bird = new Entity("Bird", rand()%(floorWidth-(32 * (SCALE_FACTOR / 5))), rand()%(floorHeight-(32 * (SCALE_FACTOR / 5))), 16 * (SCALE_FACTOR / 5), 16 * (SCALE_FACTOR / 5), Bird, 5, 3, 1, 50);
        bird->attributes.push_back(AutoWandering);
        bird->attributes.push_back(Carryable);
        bird->attributes.push_back(Fearful);
        stage->eManager.entities.push_back(bird);
    }

    int numCats = rand() % 50;
    for (int i = 0; i < numCats; ++i) {
        Entity *cat = new Entity("Cat", rand()%(floorWidth-(32 * (SCALE_FACTOR / 5))), rand()%(floorHeight-(32 * (SCALE_FACTOR / 5))), 16 * (SCALE_FACTOR / 5), 16 * (SCALE_FACTOR / 5), Cat, 15, 5, 5, 20);
        cat->attributes.push_back(AutoWandering);
        cat->attributes.push_back(Carryable);
        cat->attributes.push_back(Fearful);
        stage->eManager.entities.push_back(cat);
    }

    return stage;
}
