//#ifndef SPRITEANIMATION_H
//#define SPRITEANIMATION_H

#include <string>
#include "SDL.h"
#include "Animation.h"

class SpriteAnimation : public Animation {

 public:
     SpriteAnimation(bool repeat = true);
     void Draw(float x, float y, int spriteWidth, int SpriteHeight, float xScale= 1, float yScale=1, SDL_RendererFlip flip = SDL_FLIP_NONE);
     void SetProps(std::string textureID, int spriteRow, int frameCount, int speed);
     void Update(float dt);
/*
     int GetCurrFrame(){return m_CurrentFrame;}
     int GetCurrSpriteRow(){return m_SpriteRow;}

     void SetSpriteRow(int row){m_SpriteRow = row;}
     void IncrSpriteRow(){m_SpriteRow++;}
     void DecrSpriteRow(){m_SpriteRow--;} */

 private:
    int m_Speed;
    int m_SpriteRow;
    int m_FrameCount;
    std::string m_TextureID;
};
