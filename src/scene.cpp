#include "scene.hpp"


Scene::Scene(SceneType type,Runnable& manipulator): manipulator(manipulator)
{
    this->type=type;
}

void Scene::addButton(Button *btn)
{
    button.push_back(btn);
}

void Scene::addComponent(Bar *br)
{
    bar.push_back(br);
}

void Scene::setButtonViewport(SDL_Rect *viewport)
{
    button_viewport=viewport;
}

void Scene::setComponentViewport(SDL_Rect *viewport)
{
    component_viewport=viewport;
}

void Scene::init(SDL_Renderer *render)
{
    if(button.size()==0)
    return;
    SDL_RenderSetViewport(render,button_viewport);
    for(auto i:button)
    {
        i->render(render);
    }
}

void Scene::run(SDL_Renderer *render)
{
    if(type==SCENE_BAR)
    {
        SDL_RenderSetViewport(render,component_viewport);

        if(is_animation&&!user_interrupt)
        animateBar();

        for(auto i:bar)
        {
            i->render(render);
        }

        if(is_animation==true&&id1!=-1&&id2!=-1)
        {
            bar[id1]->render(render);
            bar[id2]->render(render);
        }

        if(start_algorithm&&!user_interrupt)
        {
            if(algo==SELECTION_SORT)
            selection_step();
            else if(algo==BUBBLE_SORT)
            bubble_step();
            else if(algo==INSERTION_SORT)
            insertion_step();
        }

        manipulator.run();
    }

    if(type==SCENE_GRAPH)
    {
        SDL_RenderSetViewport(render,component_viewport);
        for(auto i:node)
        {
            for(auto j:i)
            {
                j->render(render);
            }
        }
        if(start_algorithm==true&&!user_interrupt)
        {
            dijkstra_step();
        }
        else if(is_path_available==true&&!user_interrupt)
        {
            drawPath();
        }
    }
    
}

void Scene::handleEvent(SDL_Event e)
{
    for(auto i:button)
    {
        i->onClick(e,button_viewport->x,button_viewport->y);
    }
    if(type==SCENE_GRAPH)
    {
        bool clicked=false;
        if(node_type==START_NODE&&!user_interrupt)
        {
            for(int i=0;i<node.size();i++)
            {
                for(int j=0;j<node[i].size();j++)
                {
                    clicked=node[i][j]->onClick(e,node_type);
                    if(clicked)
                    {
                        if(start_node.first!=-1&&start_node.second!=-1)
                        node[start_node.first][start_node.second]->startNode=false;

                        if(start_node.first==i&&start_node.second==j)
                        {
                            start_node.first=-1;
                            start_node.second=-1;
                        }
                        else
                        {
                            start_node.first=i;
                            start_node.second=j;
                        }
                        std::cout<<start_node.first<<" "<<start_node.second<<"\n";
                        clicked=false;
                    }
                }
            }
        }
        else if(node_type==TARGET_NODE&&!user_interrupt)
        {
            for(int i=0;i<node.size();i++)
            {
                for(int j=0;j<node[i].size();j++)
                {
                    clicked=node[i][j]->onClick(e,node_type);
                    if(clicked)
                    {
                        if(target_node.first!=-1&&target_node.second!=-1)
                        node[target_node.first][target_node.second]->targetNode=false;
                        
                        if(target_node.first==i&&target_node.second==j)
                        {
                            target_node.first=-1;
                            target_node.second=-1;
                        }
                        else
                        {
                            target_node.first=i;
                            target_node.second=j;
                        }

                        std::cout<<target_node.first<<" "<<target_node.second<<"\n";
                        clicked=false;
                    }
                }
            }
        }
        else if(node_type==BLOCK_NODE&&!user_interrupt)
        {
            for(auto i:node)
            {
                for(auto j:i)
                {
                    j->onClick(e,node_type);
                }
            }
        }
        
    }
}

void Scene::makeVCenter(int gap)
{
    int temp_x,temp_y;
    int temp_w=button[0]->getW(),temp_h=button[0]->getH();
    int num=(int)button.size();
    std::cout<<num<<"\n";
    temp_x=(button_viewport->w/2)-(temp_w/2);
    temp_y=(button_viewport->h/2)-(temp_h*num+(gap*(num-1)))/2;
    for(int i=0;i<num;i++)
    {
        temp_y+=i==0?0:(temp_h+gap);
        std::cout<<temp_y<<"\n";
        button[i]->setPosition(temp_x,temp_y);
    }
}

void Scene::makeHCenter(int gap)
{
    int temp_x,temp_y;
    int temp_w=button[0]->getW(),temp_h=button[0]->getH();
    int num=(int)button.size();
    temp_x=(button_viewport->w/2)-(temp_w*num+(gap*(num-1)))/2;
    temp_y=(button_viewport->h/2)-(temp_h/2);

    for(int i=0;i<num;i++)
    {
        temp_x+=i==0?0:(temp_w+gap);
        button[i]->setPosition(temp_x,temp_y);
    }
}

void Scene::makeBarCenter()
{
    if(type==SCENE_BAR)
    {
        int temp_n=(int)bar.size(),temp_width,temp_height,temp_x,total_x,maximum=-1;
        
        temp_width=(int)((component_viewport->w)-temp_n+1)/temp_n;
        temp_width=temp_width>30?30:temp_width;
        total_x=temp_width*temp_n+temp_n-1;
        for(auto i:bar)
        {
            if(maximum<i->value)
            maximum=i->value;
        }

        for(auto i:bar)
        {
            temp_height=(component_viewport->h*i->value)/maximum;
            i->setSize(temp_width,temp_height);
        }

        temp_x=(component_viewport->w/2)-(total_x/2);

        for(int i=0;i<temp_n;i++)
        {
            temp_x+=i==0?0:(temp_width+1);
            bar[i]->setPosition(temp_x,0);
        }
        
    }
}

void Scene::randomizeBar()
{
    int temp_n=(int)bar.size();
    int temp_data;
    srand(time(NULL));
    for(int i=0;i<temp_n;i++)
    {
        temp_data=1+rand()%150;
        bar[i]->value=temp_data;
    }
    makeBarCenter();
}

 //Animation Implementation
void Scene::animateBar()
{
    if(id1==-1||id2==-1)
    return;

    if(is_first)
    {
        pos_id1=bar[id1]->getX();
        pos_id2=bar[id2]->getX();
        if(bar[id1]->getX()>bar[id2]->getX())
        {
            increment1=-3;
            increment2=3;
            is_first=false;
        }
        else if(bar[id1]->getX()<bar[id2]->getX())
        {
            increment1=3;
            increment2=-3;
            is_first=false;
        }
        else
        {
            is_animation=false;
            pos_id1=-1;
            pos_id2=-1;
        }

        return;
    }

    bar[id1]->setColor(255,0,0,0);
    bar[id2]->setColor(0,255,0,0);
    bar[id1]->setPosition(bar[id1]->getX()+increment1,0);
    bar[id2]->setPosition(bar[id2]->getX()+increment2,0);

    if(pos_id1>pos_id2)
    {
        if(bar[id1]->getX()<=pos_id2&&bar[id2]->getX()>=pos_id1)
        {
            bar[id1]->setColor(0,0,0,0);
            bar[id2]->setColor(0,0,0,0);
            bar[id1]->setPosition(pos_id2,0);
            bar[id2]->setPosition(pos_id1,0);
            is_animation=false;
            id1=id2=-1;
            pos_id1=pos_id2=increment1=increment2=0;
            is_first=true;
        }
    }
    else if(pos_id1<pos_id2)
    {
        if(bar[id1]->getX()>=pos_id2&&bar[id2]->getX()<=pos_id1)
        {
            bar[id1]->setColor(0,0,0,0);
            bar[id2]->setColor(0,0,0,0);
            bar[id1]->setPosition(pos_id2,0);
            bar[id2]->setPosition(pos_id1,0);
            is_animation=false;
            id1=id2=-1;
            pos_id1=pos_id2=increment1=increment2=0;
            is_first=true;
        }
    }
    
}


void Scene::reset()
{
    user_interrupt=false;
    start_algorithm=false;
    is_animation=false;
    is_first=true;
    first_step=true;
    step_done=false;

    if(type==SCENE_BAR)
    {
        i=j=n_min=index_min=count=0;
        id1=id2=-1;
        pos_id1=pos_id2=increment1=increment2=0;
        for(auto i:bar)
        {
            i->setColor(0,0,0,0);
        }

        randomizeBar();
    }
    else if(type==SCENE_GRAPH)
    {
        for(auto i:node)
        {
            for(auto j:i)
            {
                j->setColor(43,36,56,0);
                j->startNode=false;
                j->targetNode=false;
                j->blocked=false;
                j->visited=false;
                j->path=false;
                j->current_cost=9999;
                j->p_data.first=-1;
                j->p_data.second=-1;
            }
        }
        start_node.first=-1;
        start_node.second=-1;
        target_node.first=-1;
        target_node.second=-1;
        node_type=NORMAL_NODE;
        c_node_x=c_node_y=-1;
        min_node_x=min_node_y=-1;
        c_min=9999;
        is_path_available=false;
    }
}

//Graph Node Implementation

void Scene::createNode(int size)
{
    std::vector<Node*> n_data;
    int h_n=component_viewport->h/(size+2);
    int w_n=component_viewport->w/(size+2);
    int temp_gap=2;

    int temp_x=(component_viewport->w/2)-(size*w_n+temp_gap*w_n-temp_gap)/2;
    int temp_y=(component_viewport->h/2)-(size*h_n+temp_gap*h_n-temp_gap)/2;

    for(int i=0;i<h_n;i++)
    {
        temp_y+=i==0?0:(size+temp_gap);
        for(int j=0;j<w_n;j++)
        {
            temp_x+=j==0?0:(size+temp_gap);
            n_data.push_back(new Node());
            n_data[j]->setColor(43,36,56,0);
            n_data[j]->setSize(size);
            n_data[j]->setPosition(temp_x,temp_y);
        }
        node.push_back(n_data);
        n_data.clear();
        temp_x=(component_viewport->w/2)-(size*w_n+temp_gap*w_n-temp_gap)/2;
    }
    
}

void Scene::setNodeType(NodeClickType node_type)
{
    this->node_type=node_type;
}

//Algorithm Implementation

void Scene::set_Algorithm(Algorithm algo)
{
    this->algo=algo;
}

void Scene::bubble_step()
{
    int comp=(int)bar.size();

    if(start_algorithm==false||is_animation==true)
    {
        return;
    }
    
    if(i>=comp-1)
    {
        start_algorithm=false;
        step_done=true;
        i=j=0;

        for(auto i:bar)
        {
            std::cout<<i->value<<"\n";
        }
    }

    if(j>=comp-1)
    {
        i++;
        j=0;
        return;
    }

    if(bar[j]->value>bar[j+1]->value)
    {
        std::swap(bar[j],bar[j+1]);
        //std::cout<<j<<"  "<<bar[j]->value<<"  "<<bar[j+1]->value<<"  \n";
        is_animation=true;
        is_first=true;
        id1=j;
        id2=j+1;
        pos_id1=bar[j]->getX();
        pos_id2=bar[j+1]->getX();
    }
    j++;
}

void Scene::selection_step()
{
    if(start_algorithm==false||is_animation==true)
    return;

    std::cout<<"Comparison: "<<count++<<"\n";

    int comp=(int)bar.size();
    if(i==0&&j==0)
    {
        n_min=bar[0]->value;
        index_min=i;

        //bar[j]->setColor(0,0,255,0);
        std::cout<<n_min<<"\n";

    }

    if(i>=comp-1)
    {
        bar[0]->setColor(0,0,0,0);
        i=j=0;
        n_min=0;
        index_min=0;
        start_algorithm=false;
        step_done=true;
        return;

        
    }

    if(j>=comp)
    {
        bar[i]->setColor(0,0,0,0);
        bar[j-1]->setColor(0,0,0,0);
        std::swap(bar[i],bar[index_min]);
        is_animation=true;
        is_first=true;
        id1=i;
        id2=index_min;
        i++;
        j=i;
        n_min=bar[i]->value;
        index_min=i;
        return ;
    }

    if(bar[j]->value<n_min)
    {
        n_min=bar[j]->value;
        index_min=j;
    }
    
    if(j!=0)
    bar[j-1]->setColor(0,0,0,0);

    bar[j]->setColor(0,0,255,0);
    j++;

}

void Scene::insertion_step()
{
    if(start_algorithm==false||is_animation==true)
    return;

    std::cout<<"Comparison: "<<count++<<"\n";

    if(i==0&&j==0)
    {
        j++;
        return;
    }

    int comp=(int)bar.size();

    if(i>comp-2)
    {
        i=j=0;
        start_algorithm=false;
        step_done=true;
        return;
    }

    if(j==0)
    {
        i++;
        j=i+1;
        return;
    }

    if(bar[j]->value<bar[j-1]->value)
    {
        std::swap(bar[j],bar[j-1]);
        is_animation=true;
        id1=j;
        id2=j-1;
    }
    else
    {
        i++;
        j=i+1;
        return;
    }

    j--;
    
}

void Scene::findMin()
{
    min_node_x=-1;
    min_node_y=-1;
    c_min=9999;
    
    for(int i=0;i<node.size();i++)
    {
        for(int j=0;j<node[i].size();j++)
        {
            if(!node[i][j]->blocked&&!node[i][j]->visited&&node[i][j]->current_cost<c_min)
            {
                c_min=node[i][j]->current_cost;
                min_node_x=i;
                min_node_y=j;
            }
        }
    }
}

void Scene::updateCost()
{
    if(c_node_x>0)
    {
        if(!node[c_node_x-1][c_node_y]->blocked&&!node[c_node_x-1][c_node_y]->visited&&node[c_node_x-1][c_node_y]->current_cost>node[c_node_x][c_node_y]->current_cost+1)
        {
            node[c_node_x-1][c_node_y]->current_cost=node[c_node_x][c_node_y]->current_cost+1;
            node[c_node_x-1][c_node_y]->p_data.first=c_node_x;
            node[c_node_x-1][c_node_y]->p_data.second=c_node_y;
        }
    }

    if(c_node_y>0)
    {
        if(!node[c_node_x][c_node_y-1]->blocked&&!node[c_node_x][c_node_y-1]->visited&&node[c_node_x][c_node_y-1]->current_cost>node[c_node_x][c_node_y]->current_cost+1)
        {
            node[c_node_x][c_node_y-1]->current_cost=node[c_node_x][c_node_y]->current_cost+1;
            node[c_node_x][c_node_y-1]->p_data.first=c_node_x;
            node[c_node_x][c_node_y-1]->p_data.second=c_node_y;
        }
    }

    if(c_node_x<=node.size()-2)
    {
        if(!node[c_node_x+1][c_node_y]->blocked&&!node[c_node_x+1][c_node_y]->visited&&node[c_node_x+1][c_node_y]->current_cost>node[c_node_x][c_node_y]->current_cost+1)
        {
            node[c_node_x+1][c_node_y]->current_cost=node[c_node_x][c_node_y]->current_cost+1;
            node[c_node_x+1][c_node_y]->p_data.first=c_node_x;
            node[c_node_x+1][c_node_y]->p_data.second=c_node_y;
        }
    }

    if(c_node_y<=node[c_node_x].size()-2)
    {
        if(!node[c_node_x][c_node_y+1]->blocked&&!node[c_node_x][c_node_y+1]->visited&&node[c_node_x][c_node_y+1]->current_cost>node[c_node_x][c_node_y]->current_cost+1)
        {
            node[c_node_x][c_node_y+1]->current_cost=node[c_node_x][c_node_y]->current_cost+1;
            node[c_node_x][c_node_y+1]->p_data.first=c_node_x;
            node[c_node_x][c_node_y+1]->p_data.second=c_node_y;
        }
    }

}

void Scene::drawPath()
{
    if(target_node.first==-1&&target_node.second==-1)
    return;

    if(node[target_node.first][target_node.second]->p_data.first==-1&&node[target_node.first][target_node.second]->p_data.second==-1)
    {
        if(count/12==0)
        {
            count++;
            return;
        }
        else if(count/12>=1)
        {
            count=0;
        }
        for(auto i:node)
        {
            for(auto j:i)
            {
                if(j->visited)
                {
                    if(j->path==true)
                    j->path=false;
                    else
                    j->path=true;
                    
                }
            }
        }

        return;
    }

    if(c_node_x==-1&&c_node_y==-1)
    {
        c_node_x=node[target_node.first][target_node.second]->p_data.first;
        c_node_y=node[target_node.first][target_node.second]->p_data.second;
    }

    node[c_node_x][c_node_y]->path=true;
    if(node[c_node_x][c_node_y]->p_data.first==-1&&node[c_node_x][c_node_y]->p_data.second==-1)
    {
        is_path_available=false;
        c_node_x=-1;
        c_node_y=-1;
    }
    else
    {
        int temp=node[c_node_x][c_node_y]->p_data.first;
        c_node_y=node[c_node_x][c_node_y]->p_data.second;
        c_node_x=temp;
    }
    
}

void Scene::dijkstra_step()
{
    if(start_algorithm==false||node_type!=NORMAL_NODE)
    return;

    if(start_node.first==-1||start_node.second==-1||target_node.first==-1||target_node.second==-1)
    return;

    if(first_step==true)
    {
        node[start_node.first][start_node.second]->current_cost=0;
        node[start_node.first][start_node.second]->startNode=true;
        node[target_node.first][target_node.second]->targetNode=true;
        c_node_x=start_node.first;
        c_node_y=start_node.second;
        first_step=false;
    }

    if(c_node_x!=target_node.first&&c_node_y!=target_node.second);
    findMin();

    if(min_node_x!=-1&&min_node_y!=-1)
    {
        c_node_x=min_node_x;
        c_node_y=min_node_y;
        node[c_node_x][c_node_y]->visited=true;
        updateCost();
    }
    std::cout<<c_node_x<<" "<<c_node_y<<"\n";
    if((min_node_x==-1&&min_node_y==-1)||(c_node_x==target_node.first&&c_node_y==target_node.second))
    {
        start_algorithm=false;
        first_step=true;
        step_done=true;
        c_node_x=-1;
        c_node_y=-1;
        is_path_available=true;
    }

}

void Scene::start()
{
    user_interrupt=false;
    if(is_animation==false&&step_done==false)
    start_algorithm=true;

    if(step_done==true&&type==SCENE_GRAPH)
    {
        start_algorithm=true;
        step_done=false;
        is_path_available=false;

        for(auto i:node)
        {
            for(auto j:i)
            {
                    j->visited=false;
                    j->current_cost=9999;
                    j->p_data.first=-1;
                    j->p_data.second=-1;
                    j->path=false;
            }
        }
    }
}

void Scene::stop()
{
    if(start_algorithm==true)
    user_interrupt=true;
    if(is_path_available==true&&start_algorithm==false)
    user_interrupt=true;
}

