#include "Renderer.h"

#include <cstdlib>
#include <cmath>
#include <iostream>
#include <string>
#include <sstream>
#include <assert.h>

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

#include "Texture.h"
#include "RenderModel.h"
#include "json.cpp"

using json = nlohmann::json;

using namespace std;

#define WINDOW_WIDTH    640
#define WINDOW_HEIGHT   480

extern bool test;

Renderer::Renderer() {
    if (!test) {
        win = SDL_CreateWindow("Clay", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
        rend = SDL_CreateRenderer(win, -1, SDL_RENDERER_PRESENTVSYNC);
    }
    winWidth = WINDOW_WIDTH;
    winHeight = WINDOW_HEIGHT;
    realWidth = -1;
    realHeight = -1;

    if (!test) {
        //Load Textures
        textures.clear();
        for (unsigned long i = 0; i < texPaths.size(); ++i) {
            SDL_Surface *sur = IMG_Load(texPaths[i].c_str());
            if (sur == NULL) {
                cerr << "Failed to open texture: " << texPaths[i] << endl;
                assert(0);
            }
            SDL_Texture *tex = SDL_CreateTextureFromSurface(rend, sur);
            textures.push_back(tex);
            SDL_FreeSurface(sur);
        }
    }
}

void Renderer::setRenderModels(vector<RenderModel> models) {
    renderables.clear();
    for (unsigned long i = 0; i < models.size(); ++i) {
        renderables.push_back(models[i]);
    }
}

RenderModel Renderer::getFollowing() {
    for (unsigned long i = 0; i < renderables.size(); ++i) {
        if (renderables[i].following) {
            return renderables[i];
        }
    }
    bool atLeastOneFollowing = false;
    assert(atLeastOneFollowing);
    return renderables[0];
}

bool Renderer::isPointInRect(int rX, int rY, int x, int y, int w, int h) {
    if (rX >= x && rX <= (x+w) && rY >= y && rY <= (y+h)) {
        return true;
    }
    return false;
}

bool Renderer::isRectInRect(const SDL_Rect &r1, const SDL_Rect &r2) {
    if (r2.y > (r1.y + r1.h)) {
        return false;
    } else if ((r2.y + r2.h) < r1.y) {
        return false;
    } else if (r2.x > (r1.x + r1.w)) {
        return false;
    } else if ((r2.x + r2.w) < r1.x) {
        return false;
    }
    return true;
}

void Renderer::getDispRect(SDL_Rect *r) {
    //Get x and y coordinates of the begining of the display window as it
    //overlays onto the real rectangle.
    RenderModel player = getFollowing();
    int dX = player.rRect.x - (int)round((float)winWidth / 2.0);
    int dY = player.rRect.y - (int)round((float)winHeight / 2.0);
    r->x = dX;
    r->y = dY;
    r->w = winWidth;
    r->h = winHeight;
}

void Renderer::setRenderableDispCoords(int idx) {
    //Get x and y coordinates of the begining of the display window as it
    //overlays onto the real rectangle.
    RenderModel player = getFollowing();
    int dX = player.rRect.x - (int)round((float)winWidth / 2.0);
    int dY = player.rRect.y - (int)round((float)winHeight / 2.0);
    renderables[idx].dRect.x = (renderables[idx].rRect.x - dX);
    renderables[idx].dRect.y = (renderables[idx].rRect.y - dY);
}

void Renderer::renderDraw() {
    SDL_GetWindowSize(win, &winWidth, &winHeight);
    SDL_RenderClear(rend);
    
    SDL_Rect dispRect;
    getDispRect(&dispRect);

    //There is an issue with the following code with regards to entities not rendering until 
    //they are within some distance, even though they should be rendered the whole time

    // cerr << "---------------------------------------------------" << endl;
    // cerr << "x: " << dispRect.x <<  " y: " << dispRect.y << " w: " << dispRect.w << " h: " << dispRect.h << endl;
    for (unsigned long i = 0; i < renderables.size(); ++i) {
        // cerr << "x: " << renderables[i].rRect.x <<  " y: " << renderables[i].rRect.y << " w: " << renderables[i].rRect.w << " h: " << renderables[i].rRect.h;
        if (isRectInRect(renderables[i].rRect, dispRect)) {
            // cerr << " Within";
            setRenderableDispCoords(i);
            SDL_RenderCopy(rend, textures[renderables[i].tex], NULL, &(renderables[i].dRect));
        }
        // cerr << endl;
    }

    SDL_RenderPresent(rend);
}

string Renderer::serialize() {
    json j = {
        {"realWidth", realWidth},
        {"realHeight", realHeight},
    };

    return j.dump();
}

void Renderer::deserialize(string in) {
    json j;
    j = json::parse(in);
   
    realWidth = j["realWidth"];
    realHeight = j["realHeight"];

}
