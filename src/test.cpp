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
        Entity e(x, y, w, h, (TextureIdx)tex, hth, res, mvS);
        Entity obj(x, y, w, h, (TextureIdx)tex, hth, res, mvS);
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
        Entity e2(x2, y2, w2, h2, (TextureIdx)tex2, hth2, res2, mvS2);
        Entity obj2(x2, y2, w2, h2, (TextureIdx)tex2, hth2, res2, mvS2);
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

    cerr << "\033[0m";
    return allCorrect;    
}
