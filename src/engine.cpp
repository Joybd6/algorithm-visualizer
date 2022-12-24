#include "engine.hpp"

Engine::Engine():window(NULL),render(NULL)
{

}

Engine::~Engine()
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(render);
    SDL_Quit();
    IMG_Quit();
    TTF_Quit();
}

bool Engine::init()
{
    bool success=true;

    if(SDL_Init(SDL_INIT_VIDEO)<0)
    {
        success=false;
        std::cout<<"Error While Initializing SDL "<<SDL_GetError()<<"\n";
    }
    else
    {
        window=SDL_CreateWindow(utils::windowName.c_str(),SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,utils::window_w,utils::window_h,SDL_WINDOW_SHOWN);
        if(window==NULL)
        {
            success=false;
            std::cout<<"Error while creating window "<<SDL_GetError()<<"\n";
        }
        else
        {
            render=SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            if(render==NULL)
            {
                success=false;
                std::cout<<"Error while creating renderer "<<SDL_GetError()<<"\n";
            }
            else
            {
                if(!(IMG_Init(IMG_INIT_PNG)&IMG_INIT_PNG))
                {
                    success=false;
                    std::cout<<"Error while initializing SDL Image "<<IMG_GetError()<<"\n";
                }
                TTF_Init();
            }
            

        }
        
    }
}

void Engine::addScene(Scene *scene)
{
    container.push_back(scene);
}

void Engine::run()
{
    SDL_Event e;
    bool quit=false;

    while(!quit)
    {
        while(SDL_PollEvent(&e)!=0)
        {
            if(e.type==SDL_QUIT)
            quit=true;
            container[current_scene]->handleEvent(e);
        }
        SDL_SetRenderDrawColor(render,0xFF,0xFF,0xFF,0x00);
        SDL_RenderClear(render);
        container[current_scene]->init(render);
        container[current_scene]->run(render);
        SDL_RenderPresent(render);
    }
}

void Engine::setScene(int n)
{
    if(n<container.size())
    {
        current_scene=n;
    }
    else
    {
        std::cout<<"Error: scene number exceed\n";
    }
    
}

void Engine::setIcon(std::string path)
{
    SDL_Surface *temp_surface=IMG_Load(path.c_str());
    if(temp_surface!=NULL)
    {
        SDL_SetWindowIcon(window,temp_surface);
    }
    else
    {
        std::cout<<"Error while setting up Icon: "<<IMG_GetError()<<"\n";
    }
    
}

SDL_Renderer* Engine::getRenderer()
{
    return render;
}

SDL_Window* Engine::getWindow()
{
    return window;
}