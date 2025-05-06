#ifndef COLLIDER_H
#define COLLIDER_H

#include "SDL.h"

using Box = SDL_Rect;

class Collider
{
    public:
        inline Box Get(){return m_Box;}
        inline void SetBuffer(int x, int y, int w, int h){m_Buffer = {x, y, w, h};}


         void Set(int x, int y, int w, int h){
    m_Box.x = x + m_Buffer.x;
    m_Box.y = y + m_Buffer.y;
    m_Box.w = w + m_Buffer.w;
    m_Box.h = h + m_Buffer.h;
}
    private:
        Box m_Box;
        Box m_Buffer;
};

#endif // COLLIDER_H
