#include "Stage.h"
#include "json.cpp"

#include <string>

using namespace std;
using json = nlohmann::json;

Stage::Stage() {
    running = true;
}

string Stage::serialize() {
    json j = {
        {"rend", rend.serialize()},
        {"eManager", eManager.serialize()},
    };
    return j.dump();
}

void Stage::deserialize(string in) {
    json j;
    j = json::parse(in);

    rend.deserialize(j["rend"]);
    eManager.deserialize(j["eManager"]);

}

void Stage::simulate() {
    eManager.simulate();
}
