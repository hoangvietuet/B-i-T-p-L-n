/*#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <string>
#include "SDL.h"
#include "SDL_render.h"
#include "IObject.h"
#include "Transform.h"
#include "Point.h"

struct Properties {
public:
    Properties(std::string textureID, int x, int y, int width, int height, SDL_RendererFlip flip = SDL_FLIP_NONE)
        : TextureID(textureID), X(x), Y(y), Width(width), Height(height), Flip(flip) {}

public:
    std::string TextureID;
    int Width, Height;
    float X, Y;
    SDL_RendererFlip Flip;
};

class GameObject : public IObject  // Sửa IOject -> IObject
{
    public:
    Transform m_Transform; // Khai báo trước
    int m_Width;
    int m_Height;

    GameObject() : m_Transform(), m_Width(0), m_Height(0) {}

public:
    GameObject(Properties* props)
        : m_TextureID(props->TextureID),
          m_Width(props->Width),
          m_Height(props->Height),
          m_Flip(props->Flip),
          m_Transform(props->X, props->Y) {

          float px = props->X + props->Width/2;
          float py = props->Y + props->Height/2;
          m_Origin = new Point(px, py);
          }  // Không cần dùng new
    inline Point* GetOrigin(){return m_Origin;}

    virtual void Draw() = 0;
    virtual void Update(float dt) = 0;
    virtual void Clean() = 0;

protected:
    Point* m_Origin;
//    Transform m_Transform;  // Không cần con trỏ
    int m_Width, m_Height;
    std::string m_TextureID;
    SDL_RendererFlip m_Flip;
};

#endif // GAMEOBJECT_H */
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <string>
#include <memory> // Dùng cho std::unique_ptr
#include "SDL.h"
#include "SDL_render.h"
#include "IObject.h"
#include "Transform.h"
#include "Point.h"

struct Properties {
public:
    Properties(const std::string& textureID, float x, float y, int width, int height, SDL_RendererFlip flip = SDL_FLIP_NONE)
        : TextureID(textureID), X(x), Y(y), Width(width), Height(height), Flip(flip) {}

public:
    std::string TextureID;
    int Width, Height;
    float X, Y;
    SDL_RendererFlip Flip;
};

class GameObject : public IObject {
public:
    GameObject(Properties* props)
        : m_Transform(props->X, props->Y),
          m_TextureID(props->TextureID),
          m_Width(props->Width),
          m_Height(props->Height),
          m_Flip(props->Flip),
          m_Origin(std::make_unique<Point>(props->X + props->Width / 2, props->Y + props->Height / 2)) {}

    inline const Point& GetOrigin() const { return *m_Origin; }

    virtual void Draw() = 0;
    virtual void Update(float dt) = 0;
    virtual void Clean() = 0;

protected:
    Transform m_Transform;
    std::unique_ptr<Point> m_Origin;
    int m_Width, m_Height;
    std::string m_TextureID;
    SDL_RendererFlip m_Flip;
};

#endif // GAMEOBJECT_H
