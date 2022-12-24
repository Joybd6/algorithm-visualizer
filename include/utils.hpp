#ifndef UTILS_H
#define UTILS_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <string>

namespace utils
{
    const int window_w=980;
    const int window_h=620;
    const int button_w=150;
    const int button_h=60;
    const int button_gap=5;
    const std::string windowName="Algorithm Visualizer";

    const int BarNumber=60;


    class LTexture {
        private:
        SDL_Window *window=NULL;
        SDL_Texture *texture;
        SDL_Rect dimention;
        bool RectAccess=false;

        public:
        LTexture();
        LTexture(int pos_x,int pos_y,int P_width,int P_height);
        ~LTexture();

        void setX(int p_x);
        void setY(int p_y);
        void setW(int p_w);
        void setH(int p_h);
        void free();

        int getX();
        int getY();
        int getWidth();
        int getHeight();

        void setTexture(SDL_Renderer *render);
        void setBlendMode(SDL_BlendMode blendmode);
        void setAlpha(Uint8 value);

        bool loadTextureFromFile(std::string str,SDL_Renderer *render);
        bool createTextureFromString(SDL_Renderer *render,std::string str,TTF_Font *font,SDL_Color color);
    };
};

#endif