#include "utils.hpp"

utils::LTexture::LTexture()
{
    texture=NULL;
    dimention.x=0;
    dimention.y=0;
    dimention.w=-1;
    dimention.h=-1;

}

utils::LTexture::LTexture(int pos_x,int pos_y,int P_width,int P_height)
{
    texture=NULL;
    dimention.x=pos_x;
    dimention.y=pos_y;
    dimention.w=P_width;
    dimention.h=P_height;
    RectAccess=true;
}

void utils::LTexture::setX(int p_x)
{
    dimention.x=p_x;
}

void utils::LTexture::setY(int p_y)
{
    dimention.y=p_y;
}

void utils::LTexture::setW(int p_w)
{
    dimention.w=p_w;
}

void utils::LTexture::setH(int p_h)
{
    dimention.h=p_h;
}

int utils::LTexture::getX()
{
    return dimention.x;
}

int utils::LTexture::getY()
{
    return dimention.y;
}

int utils::LTexture::getWidth()
{
    return dimention.w;
}

int utils::LTexture::getHeight()
{
    return dimention.h;
}

bool utils::LTexture::loadTextureFromFile(std::string str,SDL_Renderer *render)
{
    free();
    bool success=true;
    SDL_Texture *loadedTexture=NULL;
    SDL_Surface *loadedSurface=NULL;
    //SDL_Surface *optimizedSurface=NULL;
    loadedSurface=IMG_Load(str.c_str());
    if(loadedSurface==NULL)
    {
        success=false;
        std::cout<<"Error While loading Image: "<<IMG_GetError()<<std::endl;
    }
    else
    {
        loadedTexture=SDL_CreateTextureFromSurface(render,loadedSurface);
        if(loadedTexture==NULL)
        {
            std::cout<<"Error While Converting Surface"<<SDL_GetError()<<std::endl;
            success=false;
        }
        else
        {
            dimention.w=loadedSurface->w;
            dimention.h=loadedSurface->h;
            SDL_FreeSurface(loadedSurface);
        }
        
    }

    texture=loadedTexture;

    return success;

}

bool utils::LTexture::createTextureFromString(SDL_Renderer *render,std::string str,TTF_Font *font,SDL_Color color)
{
    free();
    bool success=true;
    SDL_Surface *loadedSurface=NULL;
    SDL_Texture *loadedTexture=NULL;
    loadedSurface=TTF_RenderText_Solid(font,str.c_str(),color);
    if(loadedSurface==NULL)
    {
        success=false;
        std::cout<<"Error while Rendering Text: "<<TTF_GetError()<<std::endl;
    }   
    else
    {
        loadedTexture=SDL_CreateTextureFromSurface(render,loadedSurface);
        if(loadedTexture==NULL)
        {
            success=false;
            std::cout<<"Error while creating texture from text: "<<SDL_GetError()<<std::endl;
        }
        else
        {
            dimention.w=loadedSurface->w;
            dimention.h=loadedSurface->h;
        }
        
    }

    texture=loadedTexture;
    SDL_FreeSurface(loadedSurface);

    return success;
    
}

void utils::LTexture::setBlendMode(SDL_BlendMode blendmode)
{
    SDL_SetTextureBlendMode(texture,blendmode);
}

void utils::LTexture::setAlpha(Uint8 value)
{
    SDL_SetTextureAlphaMod(texture,value);
}

void utils::LTexture::setTexture(SDL_Renderer *render)
{
    if(dimention.h==-1||dimention.w==-1)
    SDL_RenderCopy(render,texture,NULL,NULL);
    else
    SDL_RenderCopy(render,texture,NULL,&dimention);
    
}

void utils::LTexture::free()
{
    if(texture==NULL)
    {
        SDL_DestroyTexture(texture);
        texture=NULL;
        dimention.w=0;
        dimention.h=0;
    }
}

utils::LTexture::~LTexture()
{
    free();
}
