#pragma once

#include <cstdlib>
#include <algorithm>
#include <vector>
#include <string>
#include <iostream>

#include "SDL2/SDL.h"

#include "Texture.h"
#include "Entity.h"
#include "RenderModel.h"

using namespace std;

class Renderer {
public:
    Renderer();
    string serialize();
    void deserialize(string in);

    void setRenderModels(vector<RenderModel> models);
    RenderModel getFollowing();
    void renderDraw();
    bool isPointInRect(int rX, int rY, int x, int y, int w, int h);
    bool isRectInRect(const SDL_Rect &r1, const SDL_Rect &r2);
    void getDispRect(SDL_Rect *r);
    void setRenderableDispCoords(int idx);

    SDL_Window *win;
    SDL_Renderer *rend;
    int winWidth;
    int winHeight;
    int realWidth;
    int realHeight;

    vector<RenderModel> renderables;
    vector<SDL_Texture*> textures;

    bool operator ==(const Renderer &s) {
        if (renderables.size() != s.renderables.size()) {
            return false;
        }
        for (unsigned long i = 0; i < renderables.size(); ++i) {
            if (renderables[i] != s.renderables[i]) {
                return false;
            }
        }
        return true;
    }
    
    bool operator !=(const Renderer &s) {
        return !(*this == s);
    }
};



