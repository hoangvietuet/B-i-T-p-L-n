#ifndef CAMERA_H
#define CAMERA_H

#include "SDL.h"
#include "Point.h"
#include "Vector2D.h"
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

#define MAP_WIDTH 1920
#define MAP_HEIGHT 1080
class Camera
{

    public:

       void Update(float dt);

       inline SDL_Rect GetViewBox(){return m_ViewBox;}
       inline Vector2D GetPosition(){return m_Position;}
       inline void SetTarget(const Point& target) { m_Target = &target; }
       Vector2D GetTarget() const;

    inline static Camera* GetInstance(){return s_Instance = (s_Instance != nullptr)? s_Instance: new Camera();}
    private:
        Camera(){
            m_ViewBox = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
            }


         const Point* m_Target;
        Vector2D m_Position;
         SDL_Rect m_ViewBox;
        static Camera* s_Instance;

};

#endif // CAMERA_H
