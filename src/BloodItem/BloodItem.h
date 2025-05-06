#ifndef BLOODITEM_H
#define BLOODITEM_H

#include "SDL.h"
#include "Vector2D.h"
#include "Collider.h"

class BloodItem {
public:
    BloodItem(float x, float y);
    ~BloodItem();

    void Draw();
    void Update(float dt);
    void Clean();

    SDL_Rect GetCollider() { return m_Collider->Get(); }

private:
    Vector2D m_Transform; // Vị trí
    Vector2D m_Velocity;  // Vận tốc rơi
    int m_Width, m_Height;
    std::string m_TextureID;
    Collider* m_Collider;
};

#endif // BLOODITEM_H
