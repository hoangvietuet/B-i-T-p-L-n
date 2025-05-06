#ifndef TRANSFORM_H
#define TRANSFORM_H
#include "Vector2D.h"
class Transform
{
    public:
        float X, Y;
    public:
        Transform(float x = 0, float y = 0):X(x), Y(y){}
        void Log(std::string msg = ""){
        std::cout << msg << " (X Y) = (" << X << " " << Y << ")" << std::endl;
        }


    public:
        inline void TranslateX(float x){X += x;}
        inline void TranslateY(float y){Y += y;}
        inline void Translate(Vector2D v) { X += v.X; Y += v.Y; }
        inline void SetX(float x) { X = x; }
inline void SetY(float y) { Y = y; }
inline float GetX() const { return X; }
inline float GetY() const { return Y; }

};

#endif // TRANSFORM_H
