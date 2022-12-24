#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "component.hpp"
#include "utils.hpp"
#include "scene.hpp"
#include "engine.hpp"

Engine *engine;
Scene *welcome;
Scene *sort_algo;
Scene *graph;

Bar *bar[utils::BarNumber];

Button *bubble_button;
Button *selection_button;
Button *insertion_button;
Button *dijkstra_button;

Button *start_node_button;
Button *target_node_button;
Button *block_node_button;

Button *play_button;
Button *pause_button;
Button *reset_sort_button;
Button *reset_graph_button;
Button *back_button;

//Empty Callback for scene
class SceneCallback: public Runnable
{
    public:
    void run() override;
};

void SceneCallback::run()
{
    //Do nothing
}



//Buttons Callback function
class Insertion: public Runnable
{
    public:
    void run()
    {
        SDL_SetWindowTitle(engine->getWindow(),"Insertion Sort");
        sort_algo->reset();
        sort_algo->set_Algorithm(INSERTION_SORT);
        sort_algo->makeHCenter();
        engine->setScene(1);
    }
};

class Selection: public Runnable
{
    public:
    void run()
    {
        SDL_SetWindowTitle(engine->getWindow(),"Selection Sort");
        sort_algo->reset();
        sort_algo->set_Algorithm(SELECTION_SORT);
        sort_algo->makeHCenter();
        engine->setScene(1);
    }
};

class Bubble: public Runnable
{
    public:
    void run()
    {
        SDL_SetWindowTitle(engine->getWindow(),"Bubble Sort");
        sort_algo->reset();
        sort_algo->set_Algorithm(BUBBLE_SORT);
        sort_algo->makeHCenter();
        engine->setScene(1);
    }
};

class Dijkstra: public Runnable
{
    public:
    void run()
    {
        SDL_SetWindowTitle(engine->getWindow(),"Dijkstra Algorithm");
        graph->reset();
        graph->makeHCenter(1);
        engine->setScene(2);
    }
};

//Control Button

class Play: public Runnable
{
    public:
    void run()
    {
        sort_algo->start();
        graph->setNodeType(NORMAL_NODE);
        graph->start();
    }
};

class Stop: public Runnable
{
    public:
    void run()
    {
        sort_algo->stop();
        graph->stop();
    }
};

class ResetSort: public Runnable
{
    public:
    void run()
    {
        sort_algo->reset();
    }
};

class ResetGraph: public Runnable
{
    public:
    void run()
    {
        graph->reset();
    }
};

class Back: public Runnable
{
    public: 
    void run()
    {
        SDL_SetWindowTitle(engine->getWindow(),utils::windowName.c_str());
        engine->setScene(0);
    }
};

class Start_Node: public Runnable
{
    public:
    void run()
    {
        graph->setNodeType(START_NODE);
    }
};

class Target_Node: public Runnable
{
    public:
    void run()
    {
        graph->setNodeType(TARGET_NODE);
    }
};

class Block_Node: public Runnable
{
    public:
    void run()
    {
        graph->setNodeType(BLOCK_NODE);
    }
};

void createBar()
{
    for(int i=0;i<utils::BarNumber;i++)
    {
        bar[i]=new Bar(utils::BarNumber-i);
        bar[i]->setColor(0,0,0,0);
        sort_algo->addComponent(bar[i]);
    }
}

int main(int argc,char* args[])
{
    engine=new Engine();
    engine->init();
    engine->setIcon("resources/icon.png");

    //Callback class Initialization

    SceneCallback callbackscene;
    Bubble callbackbubble;
    Selection callbackselection;
    Insertion callbackinsertion;
    Dijkstra callbackgraph;

    Start_Node callbackstartnode;
    Target_Node callbacktargetnode;
    Block_Node callbackblocknode;

    Play callbackplay;
    Stop callbackstop;
    Back callbackback;
    ResetSort callbackresetsort;
    ResetGraph callbackresetgraph;


    //Button Initialization

    bubble_button=new Button(callbackbubble);
    bubble_button->setPic("resources/button_bubble-sort.png",engine->getRenderer());
    bubble_button->setSize(180,50);

    selection_button=new Button(callbackselection);
    selection_button->setPic("resources/button_selection-sort.png",engine->getRenderer());
    selection_button->setSize(180,50);

    insertion_button=new Button(callbackinsertion);
    insertion_button->setPic("resources/button_insertion-sort.png",engine->getRenderer());
    insertion_button->setSize(180,50);

    dijkstra_button=new Button(callbackgraph);
    dijkstra_button->setPic("resources/button_dijkstra.png",engine->getRenderer());
    dijkstra_button->setSize(180,50);

    start_node_button=new Button(callbackstartnode);
    start_node_button->setPic("resources/button_start-node.png",engine->getRenderer());
    start_node_button->setSize(130,50);

    target_node_button=new Button(callbacktargetnode);
    target_node_button->setPic("resources/button_target-node.png",engine->getRenderer());
    target_node_button->setSize(130,50);

    block_node_button=new Button(callbackblocknode);
    block_node_button->setPic("resources/button_block.png",engine->getRenderer());
    block_node_button->setSize(130,50);

    play_button=new Button(callbackplay);
    play_button->setPic("resources/button_play.png",engine->getRenderer());
    play_button->setSize(130,50);

    pause_button=new Button(callbackstop);
    pause_button->setPic("resources/button_pause.png",engine->getRenderer());
    pause_button->setSize(130,50);

    reset_sort_button=new Button(callbackresetsort);
    reset_sort_button->setPic("resources/button_reset.png",engine->getRenderer());
    reset_sort_button->setSize(130,50);

    reset_graph_button=new Button(callbackresetgraph);
    reset_graph_button->setPic("resources/button_reset.png",engine->getRenderer());
    reset_graph_button->setSize(130,50);

    back_button=new Button(callbackback);
    back_button->setPic("resources/button_back.png",engine->getRenderer());
    back_button->setSize(130,50);

    //Viewport Setup

    SDL_Rect viewport_component{0,0,utils::window_w,utils::window_h-80};
    SDL_Rect viewport_button{0,utils::window_h-80,utils::window_w,80};
    SDL_Rect viewport_main{0,0,utils::window_w,utils::window_h};

    //Main menu scene initialization

    welcome=new Scene(SCENE_WELCOME,callbackscene);
    welcome->setButtonViewport(&viewport_main);
    welcome->setComponentViewport(&viewport_main);

    welcome->addButton(bubble_button);
    welcome->addButton(insertion_button);
    welcome->addButton(selection_button);
    welcome->addButton(dijkstra_button);
    welcome->makeVCenter(8);

    engine->addScene(welcome);

    //Sorting Algorithm Scene

    sort_algo=new Scene(SCENE_BAR,callbackscene);
    sort_algo->setComponentViewport(&viewport_component);
    sort_algo->setButtonViewport(&viewport_button);

    createBar();
    sort_algo->set_Algorithm(BUBBLE_SORT);
    sort_algo->makeBarCenter();

    sort_algo->addButton(play_button);
    sort_algo->addButton(pause_button);
    sort_algo->addButton(reset_sort_button);
    sort_algo->addButton(back_button);
    sort_algo->makeHCenter(5);
    engine->addScene(sort_algo);

    //Graph Algorithm Scene
    graph=new Scene(SCENE_GRAPH,callbackscene);
    graph->setComponentViewport(&viewport_component);
    graph->setButtonViewport(&viewport_button);
    graph->createNode(20);

    graph->addButton(start_node_button);
    graph->addButton(target_node_button);
    graph->addButton(block_node_button);
    graph->addButton(play_button);
    graph->addButton(pause_button);
    graph->addButton(reset_graph_button);
    graph->addButton(back_button);
    graph->makeHCenter(1);
    
    engine->addScene(graph);


    engine->run();

    return 0;
}



