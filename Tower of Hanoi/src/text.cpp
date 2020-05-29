#include "text.h"

text::text()
{
    //ctor
    text_color.r = 255;
    text_color.g = 255;
    text_color.b = 255;//
    texture = NULL;
}

text::~text()
{
    //dtor
}

bool text::loadfromrender(TTF_Font* font,SDL_Renderer* screen)
{
    SDL_Surface* text_sur = TTF_RenderText_Solid(font, str_val.c_str(), text_color);
    if(text_sur) {
        texture = SDL_CreateTextureFromSurface(screen, text_sur);
        w_ = text_sur->w;
        h_ = text_sur->h;

        SDL_FreeSurface(text_sur);
    }
    return texture!=NULL;
}

void text::Free()
{
    if(texture != NULL) {
        SDL_DestroyTexture(texture);
        texture = NULL;
    }
}

void text::setcolor(Uint8 red, Uint8 green, Uint8 blue)
{
    text_color.r = red;
    text_color.g = green;
    text_color.b = blue;
}

void text::setcolor_(int type)
{
    if(type == black) {
        SDL_Color color = {0, 0 ,0};
        text_color = color;
    }
    if(type == orange) {
        SDL_Color color = {255, 165 ,0};
        text_color = color;
    }
    if(type == red) {
        SDL_Color color = {255, 0 ,0};
        text_color = color;
    }
}
void text::rendertext(SDL_Renderer* screen,
                      int x_, int y_, SDL_Rect* clip,
                      double k, SDL_Point* center, SDL_RendererFlip flip)
{
     SDL_Rect renderquad = {x_, y_, w_, h_};
     if(clip != NULL) {
        renderquad.w = clip->w;
        renderquad.h = clip->h;
     }
     SDL_RenderCopyEx(screen, texture, clip, &renderquad, k, center, flip);
}
