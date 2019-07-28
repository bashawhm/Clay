#include "test.h"
#include "Renderer.h"
#include "Entity.h"
#include "EntityManager.h"


#include <cstdlib>
#include <cmath>
#include <iostream>
#include <string>

using namespace std;

bool test;

bool testEntitySerialization(int n) {
    bool allCorrect = true;
    for (int i = 0; i < n; ++i) {
        int x = rand();
        int y = rand();
        int w = rand();
        int h = rand();
        int tex = rand();
        int hth = rand();
        int res = rand();
        int mvS = rand();
        int att = rand();
        Entity e("", x, y, w, h, (TextureIdx)tex, hth, res, mvS);
        Entity obj("", x, y, w, h, (TextureIdx)tex, hth, res, mvS);
        e.inventory.push_back(&obj);
        e.attributes.push_back((EntityAttributes)att);
        string check = e.serialize();
        int x2 = rand();
        int y2 = rand();
        int w2 = rand();
        int h2 = rand();
        int tex2 = rand();
        int hth2 = rand();
        int res2 = rand();
        int mvS2 = rand();
        Entity e2(" ", x2, y2, w2, h2, (TextureIdx)tex2, hth2, res2, mvS2);
        Entity obj2("", x2, y2, w2, h2, (TextureIdx)tex2, hth2, res2, mvS2);
        e2.inventory.push_back(&obj2);
        e2.deserialize(check);
        if (e != e2) {
            cerr << check << endl << endl;
            allCorrect = false;
        }
    }
    return allCorrect;
}

bool testRendererSerialization(int n) {
    bool allCorrect = true;
    for (int i = 0; i < n; ++i) {
        Renderer s;
        int rW = rand();
        int rH = rand();
        s.realWidth = rW;
        s.realHeight = rH;
        string check = s.serialize();
        Renderer s2;
        int rW2 = rand();
        int rH2 = rand();
        s2.realWidth = rW2;
        s2.realHeight = rH2;
        s2.deserialize(check);
        if (s != s2) {
            allCorrect = false;
        }
    }
    return allCorrect;
}

bool testRendererRectHitBoxes(int n) {
    bool allCorrect = true;
    for (int i = 0; i < n; ++i) {
        int x = abs(rand()%65535);
        int y = abs(rand()%65535);
        int w = abs(rand()%65535);
        int h = abs(rand()%65535);
        int rX = abs(rand()%(w))+x;
        int rY = abs(rand()%(h))+y;
        Renderer s;
        bool correct = s.isPointInRect(rX, rY, x, y, w, h);
        if (!correct) {
            allCorrect = false;
        }
    }

    return allCorrect;
}

bool testRendererRectInRect(int n) {
    bool allCorrect = true;
    for (int i = 0; i < n; ++i) {
        SDL_Rect r1;
        r1.x = abs(rand()%65535);
        r1.y = abs(rand()%65535);
        r1.w = abs(rand()%65535);
        r1.h = abs(rand()%65535);
        SDL_Rect r2;
        r2.x = abs(rand()%(r1.w))+r1.x;
        r2.y = abs(rand()%(r1.h))+r1.y;
        r2.w = abs(rand()%(r1.w));
        r2.h = abs(rand()%(r1.h));
        Renderer s;
        bool correct = s.isRectInRect(r1, r2);
        if (!correct) {
            cerr << i << endl;
            allCorrect = false;
        }
    }

    return allCorrect;
}

bool testEntityManager_StartsEmpty() {
    EntityManager em;
    return em.entities.size() == 0;
}

bool testEntityManager_isEntityInEntity(int n) {
    bool allCorrect = true;
    for (int i = 0; i < n; ++i) {
        int x = abs(rand()%65535);
        int y = abs(rand()%65535);
        int w = abs(rand()%65535);
        int h = abs(rand()%65535);
        int tex = rand();
        int hth = rand();
        int res = rand();
        int mvS = rand();
        Entity e1("", x, y, w, h, (TextureIdx)tex, hth, res, mvS);
        int x2 = abs(rand()%(w))+x;
        int y2 = abs(rand()%(h))+y;
        int w2 = abs(rand()%(w));
        int h2 = abs(rand()%(h));
        Entity e2("", x2, y2, w2, h2, (TextureIdx)tex, hth, res, mvS);

        EntityManager em;
        bool correct = em.isEntityInEntity(e1, e2);
        if (!correct) {
            allCorrect = false;
        }
    }

    return allCorrect;
}

bool testEntityManager_canPickup(int n) {
    bool allCorrect = true;
    for (int i = 0; i < n; ++i) {
        int x = abs(rand()%65535);
        int y = abs(rand()%65535);
        int w = abs(rand()%65535);
        int h = abs(rand()%65535);
        int tex = rand();
        int hth = rand();
        int res = rand();
        int mvS = rand();
        Entity e1("", x, y, w, h, (TextureIdx)tex, hth, res, mvS);
        e1.attributes.push_back(Carryable);
        int x2 = abs(rand()%(w))+x;
        int y2 = abs(rand()%(h))+y;
        int w2 = abs(rand()%(w));
        int h2 = abs(rand()%(h));
        Entity e2("", x2, y2, w2, h2, (TextureIdx)tex, hth, res, mvS);
        e2.attributes.push_back(Carryable);

        EntityManager em;
        em.entities.push_back(&e1);
        em.entities.push_back(&e2);
        bool correct = (em.canPickup(e1) == &e2);
        if (!correct) {
            allCorrect = false;
        }
    }

    return allCorrect;
}

bool testEntityManager_carryable() {
    int x = abs(rand()%65535);
    int y = abs(rand()%65535);
    int w = abs(rand()%65535);
    int h = abs(rand()%65535);
    int tex = rand();
    int hth = rand();
    int res = rand();
    int mvS = rand();
    Entity e1("", x, y, w, h, (TextureIdx)tex, hth, res, mvS);
    int x2 = abs(rand()%(w))+x;
    int y2 = abs(rand()%(h))+y;
    int w2 = abs(rand()%(w));
    int h2 = abs(rand()%(h));
    Entity e2("", x2, y2, w2, h2, (TextureIdx)tex, hth, res, mvS);
    e2.attributes.push_back(Carryable);

    EntityManager em;
    em.entities.push_back(&e1);
    em.entities.push_back(&e2);
    Entity *item = em.canPickup(e1);
    em.addToInventory(&e1, item);
    if (!(item->inInventory)) {
        return false;
    }
    bool isContained = (e1.inventory[0] == item);
    return isContained;

}

bool testEntityManager_dontRenderInventoryEntitys() {
    int x = abs(rand()%65535);
    int y = abs(rand()%65535);
    int w = abs(rand()%65535);
    int h = abs(rand()%65535);
    int tex = rand();
    int hth = rand();
    int res = rand();
    int mvS = rand();
    Entity e1("", x, y, w, h, (TextureIdx)tex, hth, res, mvS);
    e1.attributes.push_back(Carryable);
    int x2 = abs(rand()%(w))+x;
    int y2 = abs(rand()%(h))+y;
    int w2 = abs(rand()%(w));
    int h2 = abs(rand()%(h));
    Entity e2("", x2, y2, w2, h2, (TextureIdx)tex, hth, res, mvS);
    e2.attributes.push_back(Carryable);

    EntityManager em;
    em.entities.push_back(&e1);
    em.entities.push_back(&e2);
    Entity *item = em.canPickup(e1);
    em.addToInventory(&e1, item);
    vector<RenderModel> models = em.getRenderModels();
    for (unsigned long i = 0; i < em.entities.size(); ++i) {
        if (em.entities[i]->inInventory) {
            for (unsigned long j = 0; j < models.size(); ++j) {
                if (models[i] == em.entities[i]->getRenderModel()) {
                    return false;
                }
            }
        }
    }
    return true;
}

bool testEntityManager_noDuplicateInventoryItems() {
    int x = abs(rand()%65535);
    int y = abs(rand()%65535);
    int w = abs(rand()%65535);
    int h = abs(rand()%65535);
    int tex = rand();
    int hth = rand();
    int res = rand();
    int mvS = rand();
    Entity e1("", x, y, w, h, (TextureIdx)tex, hth, res, mvS);
    e1.attributes.push_back(Carryable);
    int x2 = abs(rand()%(w))+x;
    int y2 = abs(rand()%(h))+y;
    int w2 = abs(rand()%(w));
    int h2 = abs(rand()%(h));
    Entity e2("", x2, y2, w2, h2, (TextureIdx)tex, hth, res, mvS);
    e2.attributes.push_back(Carryable);

    EntityManager em;
    em.entities.push_back(&e1);
    em.entities.push_back(&e2);
    Entity *item = em.canPickup(e1);
    em.addToInventory(&e1, item);
    em.addToInventory(&e1, item);
    if (em.entities[0]->inventory.size() > 1) {
        return false;
    }
    return true;
}

bool runAllTests(int n) {
    test = true;
    srand(time(NULL));
    bool allCorrect = true;
    bool correct;
    correct = testEntitySerialization(n);
    if (!correct) {
        cerr << "\033[1;31mFailed entity serialization test" << endl;
        allCorrect = false;
    } else {
        cerr << "\033[1;32mPassed entity serialization test" << endl;
    }
    correct = testRendererSerialization(n);
    if (!correct) {
        cerr << "\033[1;31mFailed Renderer serialization test" << endl;
        allCorrect = false;
    } else {
        cerr << "\033[1;32mPassed Renderer serialization test" << endl;
    }
    correct = testRendererRectHitBoxes(n);
    if (!correct) {
        cerr << "\033[1;31mFailed Renderer rect bounding box test" << endl;
        allCorrect = false;
    } else {
        cerr << "\033[1;32mPassed Renderer rect bounding box test" << endl;
    }
    correct = testRendererRectInRect(n);
    if (!correct) {
        cerr << "\033[1;31mFailed Renderer rect in rect test" << endl;
        allCorrect = false;
    } else {
        cerr << "\033[1;32mPassed Renderer rect in rect test" << endl;
    }
    correct = testEntityManager_StartsEmpty();
    if (!correct) {
        cerr << "\033[1;31mFailed EntityManager_StartsEmpty test" << endl;
        allCorrect = false;
    } else {
        cerr << "\033[1;32mPassed EntityManager_StartsEmpty test" << endl;
    }
    correct = testEntityManager_isEntityInEntity(n);
    if (!correct) {
        cerr << "\033[1;31mFailed EntityManager_isEntityInEntity test" << endl;
        allCorrect = false;
    } else {
        cerr << "\033[1;32mPassed EntityManager_isEntityInEntity test" << endl;
    }
    correct = testEntityManager_canPickup(n);
    if (!correct) {
        cerr << "\033[1;31mFailed EntityManager_canPickup test" << endl;
        allCorrect = false;
    } else {
        cerr << "\033[1;32mPassed EntityManager_canPickup test" << endl;
    }
    correct = testEntityManager_carryable();
    if (!correct) {
        cerr << "\033[1;31mFailed EntityManager_carryable test" << endl;
        allCorrect = false;
    } else {
        cerr << "\033[1;32mPassed EntityManager_carryable test" << endl;
    }
    correct = testEntityManager_dontRenderInventoryEntitys();
    if (!correct) {
        cerr << "\033[1;31mFailed EntityManager_dontRenderInventoryEntitys test" << endl;
        allCorrect = false;
    } else {
        cerr << "\033[1;32mPassed EntityManager_dontRenderInventoryEntitys test" << endl;
    }
    correct = testEntityManager_noDuplicateInventoryItems();
    if (!correct) {
        cerr << "\033[1;31mFailed testEntityManager_noDuplicateInventoryItems test" << endl;
        allCorrect = false;
    } else {
        cerr << "\033[1;32mPassed testEntityManager_noDuplicateInventoryItems test" << endl;
    }

    cerr << "\033[0m";
    return allCorrect;    
}
