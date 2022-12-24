#ifndef COMPONENT_H
#define COMPONENT_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <string>
#include "utils.hpp"

class Component {
    public:
    virtual void render(SDL_Renderer *render)=0;
};

class Runnable 
{
    public:
    virtual void run()=0;
};

class Button: public Component {
    private:
    utils::LTexture texture;
    int x,y,width,height;
    Runnable& callback;
    Uint8 r,g,b;
    double text_outline;
    std::string button_text;
    TTF_Font *font;

    public:
    Button(Runnable &dt);

    void setPosition(int x,int y);
    void setSize(int width,int height);
    void setPic(std::string path,SDL_Renderer *render);
    void setText(std::string text,TTF_Font *font,double outline,Uint8 r,Uint8 g,Uint8 b);
    int getX();
    int getY();
    int getW();
    int getH();

    void render(SDL_Renderer *render) override;
    void onClick(SDL_Event e,int viewport_x,int viewport_y);
};

class Bar: public Component
{
    private:
    int  x,y,width,height;
    Uint8 r=0,g=0,b=0,a=255;
    public:
    int value;
    Bar(int value);
    Bar(int value,int width,int height);
    void render(SDL_Renderer *render) override;
    void setPosition(int x,int y);
    void setSize(int width,int height);
    void setColor(Uint8 r,Uint8 g,Uint8 b,Uint8 a);

    int getX();
    int getY();
    int getW();
    int getH();


};

enum NodeClickType{NORMAL_NODE,START_NODE,TARGET_NODE,BLOCK_NODE};
typedef std::pair<int,int> parent;

class Node: public Component
{
    private:
    bool is_hover=false;
    int size;
    int x,y;
    Uint8 r,g,b,a;
    Uint8 blocked_r=130,blocked_g=130,blocked_b=130,blocked_a=255;
    Uint8 visited_r=17,visited_g=212,visited_b=219,visited_a=255;
    Uint8 start_r=0,start_g=255,start_b=0,start_a=255;
    Uint8 target_r=255,target_g=0,target_b=0,target_a=255;
    Uint8 path_r=125,path_g=0,path_b=255,path_a=255;

    void setCurrentColor(NodeClickType c_type);
    public:
    
    int current_cost=9999;
    bool blocked=false;
    bool visited=false;
    bool startNode=false;
    bool targetNode=false;
    bool path=false;
    parent p_data{-1,-1};

    void render(SDL_Renderer *render) override;
    bool onClick(SDL_Event e,NodeClickType clickType);
    void setColor(Uint8,Uint8,Uint8,Uint8);

    void setSize(int size);
    void setPosition(int x,int y);
    int getX();
    int getY();
    int getSize();
    

};

#endif