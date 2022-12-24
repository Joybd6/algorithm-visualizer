#include "component.hpp"

//Button Class Implementation

Button::Button(Runnable &dt): callback(dt),font(NULL)
{
    text_outline=0;
}

void Button::setPosition(int x,int y)
{
    this->x=x;
    this->y=y;
}

void Button::setSize(int width,int height)
{
    this->width=width;
    this->height=height;
}

void Button::setText(std::string text,TTF_Font *font,double outline,Uint8 r,Uint8 g,Uint8 b)
{
    this->button_text=text;
    this->text_outline=0;
    this->r=r;
    this->g=g;
    this->b=b;


}

void Button::setPic(std::string path,SDL_Renderer *render)
{
    texture.loadTextureFromFile(path,render);
}

void Button::render(SDL_Renderer *render)
{
    texture.setX(x);
    texture.setY(y);
    texture.setW(width);
    texture.setH(height);
    texture.setTexture(render);
}

int Button::getX()
{
    return x;
}

int Button::getY()
{
    return y;
}

int Button::getW()
{
    return width;
}

int Button::getH()
{
    return height;
}

void Button::onClick(SDL_Event e,int viewport_x,int viewport_y)
{
    if(e.type==SDL_MOUSEBUTTONDOWN)
    {
        int mouse_x,mouse_y;
        SDL_GetMouseState(&mouse_x,&mouse_y);

        if(mouse_x>=x+viewport_x&&mouse_x<=x+width+viewport_x&&mouse_y>=y+viewport_y&&mouse_y<=y+height+viewport_y)
        {
            callback.run();
        }
    }
}

//Bar Class Implementation

Bar::Bar(int value)
{
    this->value=value;
}

Bar::Bar(int value,int width,int height)
{
    this->value=value;
    this->width=width;
    this->height=height;
}

void Bar::setPosition(int x,int y)
{
    this->x=x;
    this->y=y;
}

void Bar::setSize(int width,int height)
{
    this->width=width;
    this->height=height;
}

void Bar::setColor(Uint8 r,Uint8 g,Uint8 b,Uint8 a)
{
    this->r=r;
    this->g=g;
    this->b=b;
    this->a=a;
}

int Bar::getX()
{
    return x;
}

int Bar::getY()
{
    return y;
}

int Bar::getW()
{
    return width;
}

int Bar::getH()
{
    return height;
}

void Bar::render(SDL_Renderer *render)
{
    SDL_Rect rect{x,y,width,height};
    SDL_SetRenderDrawColor(render,r,g,b,a);
    SDL_RenderFillRect(render,&rect);
    SDL_SetRenderDrawColor(render,0xFF,0xFF,0xFF,0x00);
}

//Node class implementation

void Node::render(SDL_Renderer *render)
{

    if(startNode==false&&targetNode==false&&blocked==false&&visited==false)
    {
        SDL_SetRenderDrawColor(render,r,g,b,a);
    }
    else if(startNode==true&&targetNode==false&&blocked==false)
    {
        SDL_SetRenderDrawColor(render,start_r,start_g,start_b,start_a);
    }
    else if(targetNode==true&&startNode==false&&blocked==false)
    {
        SDL_SetRenderDrawColor(render,target_r,target_g,target_b,target_a);
    }
    else if(blocked==true&&startNode==false&&targetNode==false)
    {
        SDL_SetRenderDrawColor(render,blocked_r,blocked_g,blocked_b,blocked_a);
    }
    else if(path==true&&startNode==false&&targetNode==false&&blocked==false)
    {
        SDL_SetRenderDrawColor(render,path_r,path_g,path_b,path_a);
    }
    else if(visited==true&&startNode==false&&targetNode==false&&blocked==false)
    {
        SDL_SetRenderDrawColor(render,visited_r,visited_g,visited_b,visited_a);
    }

    SDL_Rect rect{x,y,size,size};
    SDL_RenderFillRect(render,&rect);
    SDL_SetRenderDrawColor(render,255,255,255,0);
}

bool Node::onClick(SDL_Event e,NodeClickType clickType)
{
    bool clicked=false;
    int c_x,c_y;
    SDL_GetMouseState(&c_x,&c_y);
    if(c_x>=x&&c_x<=x+size&&c_y>=y&&c_y<=y+size)
    {
        if(e.type==SDL_MOUSEBUTTONDOWN)
        {
            if(clickType==START_NODE)
            {
                if(targetNode==false)
                {
                    startNode=true;
                    blocked=false;
                    clicked=true;
                }
            }
            else if(clickType==TARGET_NODE)
            {
                if(startNode==false&&blocked==false)
                {
                    targetNode=true;
                    blocked=false;
                    clicked=true;
                }
            }
            else if(clickType==BLOCK_NODE)
            {
                if(startNode==false&&targetNode==false)
                {
                    if(blocked==false)
                    blocked=true;
                    else
                    blocked=false;
                }
            }
        }
    }
    else
    {
        setColor(43,36,56,0);
    }

    return clicked;
    
}

void Node::setColor(Uint8 r,Uint8 g,Uint8 b,Uint8 a)
{
    this->r=r;
    this->g=g;
    this->b=b;
    this->a=a;
}

int Node::getX()
{
    return x;
}

int Node::getY()
{
    return y;
}

int Node::getSize()
{
    return size;
}

void Node::setPosition(int x,int y)
{
    this->x=x;
    this->y=y;
}

void Node::setSize(int size)
{
    this->size=size;
}






