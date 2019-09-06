#pragma once

#include "Texture.h"

#include "SDL2/SDL.h"

class RenderModel {
public:
    RenderModel(const SDL_Rect &r, const SDL_Rect &d, bool f, TextureIdx t, int mvS) {
        rRect = r;
        dRect = d;
        following = f;
        tex = t;
        moveSpeed = mvS;
    }

    SDL_Rect rRect;
    SDL_Rect dRect;
    bool following;
    TextureIdx tex;
    int moveSpeed;

    RenderModel& operator =(const RenderModel &r) {
        rRect = r.rRect;
        dRect = r.dRect;
        tex = r.tex;
        moveSpeed = r.moveSpeed;
        return *this;
    }

    bool operator ==(const RenderModel &m) {
        if (rRect.x != m.rRect.x) {
            return false;
        } else if (rRect.y != m.rRect.y) {
            return false;
        } else if (rRect.w != m.rRect.w) {
            return false;
        } else if (rRect.h != m.rRect.h) {
            return false;
        } else if (dRect.x != m.dRect.x) {
            return false;
        } else if (dRect.y != m.dRect.y) {
            return false;
        } else if (dRect.w != m.dRect.w) {
            return false;
        } else if (dRect.h != m.dRect.h) {
            return false;
        } else if (following != m.following) {
            return false;
        } else if (tex != m.tex) {
            return false;
        } else if (moveSpeed != m.moveSpeed) {
            return false;
        } 
        return true;
    }
    
    bool operator !=(const RenderModel &m) {
        return !(*this == m);
    }
};
