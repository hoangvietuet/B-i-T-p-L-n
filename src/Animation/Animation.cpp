#include "Animation.h"
#include "TextureManager.h"

void Animation::Update()
{
    m_SpriteFrame = (SDL_GetTicks()/m_AnimSpeed) % m_FrameCount;

}

void Animation::Draw(float x, float y, int spritWidth, int spriteHeight, SDL_RendererFlip flip)
{
    TextureManager::GetInstance()->DrawFrame(m_TextureID, x, y, spritWidth, spriteHeight, m_SpriteRow, m_SpriteFrame, m_Flip);
}

void Animation::SetProps(std::string textureID, int spriteRow, int frameCount, int animSpeed, SDL_RendererFlip flip)
{
   m_TextureID = textureID;
   m_SpriteRow = spriteRow;
   m_FrameCount = frameCount;
   m_AnimSpeed = animSpeed;
   m_Flip = flip;

}
void Animation::SetFrameSize(int width, int height) {
    m_FrameWidth = width;
    m_FrameHeight = height;
}
