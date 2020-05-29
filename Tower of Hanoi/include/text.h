#ifndef TEXT_H
#define TEXT_H

#include "print.h"

class text
{
    public:
        text();
        virtual ~text();
        enum textcolor
        {
            black = 0,
            orange = 1,
            red = 2,
        };
        bool loadfromrender(TTF_Font* font, SDL_Renderer* screen);
        void Free();
        void setcolor(Uint8 red, Uint8 green, Uint8 blue);
        void setcolor_(int type);
        void rendertext(SDL_Renderer* screen,
                      int x_, int y_, SDL_Rect* clip=NULL,
                      double k=0.0, SDL_Point* center=NULL, SDL_RendererFlip flip=SDL_FLIP_NONE);
        void settext( const string& text) {str_val = text;}
        string gettext() const {return str_val;}
    protected:

    private:
        string str_val;
        SDL_Color text_color;
        SDL_Texture* texture;
        int w_;
        int h_;
};

#endif // TEXT_H
