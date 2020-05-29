#ifndef PRINT_H
#define PRINT_H

#include <bits/stdc++.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

using namespace std;

class Print
{
    public:
        Print();
        virtual ~Print();
        int a;
        int b;
        void SetRect(const int& x, const int& y) {rect_.x = x, rect_.y = y;}
        SDL_Rect GetRect() const {return rect_;}
        SDL_Texture* GetObject() const {return p_object_;}

        bool LoadImg(string path, SDL_Renderer* screen);
        void Render(SDL_Renderer* des, const SDL_Rect* clip = NULL);
        void Free();
    protected:
        SDL_Texture* p_object_;
        SDL_Rect rect_;


};

#endif // PRINT_H
