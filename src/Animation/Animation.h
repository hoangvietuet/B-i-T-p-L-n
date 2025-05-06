#ifndef ANIMATION_H
#define ANIMATION_H
#include <string>
#include "SDL.h"
class Animation
{
    public:
        Animation(){}
       void Update();
        void Draw(float x, float y, int spritWidth, int spriteHeight, SDL_RendererFlip flip = SDL_FLIP_NONE);
        void SetProps(std::string textureID, int spritRow, int frameCount, int animSpeed, SDL_RendererFlip flip = SDL_FLIP_NONE);
        void SetFrameSize(int width, int height);
    private:
        int  m_SpriteRow, m_SpriteFrame;
       int m_AnimSpeed, m_FrameCount;
        std::string m_TextureID;
       SDL_RendererFlip m_Flip;
       int m_FrameWidth, m_FrameHeight;

};

#endif // ANIMATION_H
