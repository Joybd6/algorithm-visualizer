#ifndef SCENE_H
#define SCENE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>

#include "component.hpp"
#include "utils.hpp"

enum SceneType{SCENE_BAR,SCENE_GRAPH,SCENE_WELCOME};
enum Algorithm{BUBBLE_SORT,SELECTION_SORT,INSERTION_SORT,MERGE_SORT,DIJKSTRA};
enum CenterType{SCENE_VERTICAL,SCENE_HORIZONTAL};

class Scene 
{
    private:
    //algorithm variable
    bool user_interrupt=false;
    int i=0,j=0;
    int n_min=0;
    int index_min=0;
    bool start_algorithm=true;
    bool first_step=true;
    bool step_done=false;
    int count=0;
    std::vector<std::vector<int>> node_cost;

    //Component Variable
    std::vector<Button*> button;
    std::vector<Bar*> bar;
    std::vector<std::vector<Node*>> node;
    std::pair<int,int> start_node{-1,-1};
    std::pair<int,int> target_node{-1,-1};

    //viewport
    SDL_Rect *button_viewport=NULL;
    SDL_Rect *component_viewport=NULL;

    //types
    SceneType type;
    Algorithm algo=INSERTION_SORT;
    NodeClickType node_type=NORMAL_NODE;

    //Animation Variable
    bool is_animation=false;
    bool is_first=true;
    int id1=-1,id2=-1,pos_id1=0,pos_id2=0;
    int increment1=0,increment2=0;
    Runnable &manipulator;

    //dijkstra algorithm variable and function
    bool is_path_available=false;
    int c_node_x=-1,c_node_y=-1;
    int min_node_x=-1,min_node_y=-1;
    int c_min=9999;
    void findMin();
    void updateCost();
    



    public:

    Scene(SceneType type,Runnable& manipulator);
    void init(SDL_Renderer *render);
    void addButton(Button *btn);
    void addComponent(Bar *cmpt);

    //viewport Function
    void setButtonViewport(SDL_Rect *viewport);
    void setComponentViewport(SDL_Rect *viewport);

    //clear function
    void reset();

    //core function
    void run(SDL_Renderer *render);
    void handleEvent(SDL_Event e);
    void makeVCenter(int gap=2);
    void makeHCenter(int gap=2);
    void makeBarCenter();
    void randomizeBar();

    //Graph Node Function
    void createNode(int size);
    void setNodeType(NodeClickType node_type);

    //Algorithm Step
    void set_Algorithm(Algorithm algo);
    void bubble_step();
    void selection_step();
    void insertion_step();
    void merge_step();
    void start();
    void stop();

    //dijkstra algorithm
    void dijkstra_step();
    void drawPath();
    

    //Animation
    void animateBar();

};

#endif