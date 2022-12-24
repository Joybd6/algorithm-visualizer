#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include "utils.hpp"
#include "component.hpp"
#include "scene.hpp"


class Engine 
{
    private:
    SDL_Window *window;
    SDL_Renderer *render;
    std::vector<Scene*> container;
    int current_scene=0;

    public:
    Engine();
    ~Engine();

    bool init();
    void addScene(Scene *scene);
    void run();
    void setScene(int n);
    void setIcon(std::string path);
    SDL_Renderer* getRenderer();
    SDL_Window *getWindow();

};