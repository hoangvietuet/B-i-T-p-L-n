#ifndef VECTOR2D_H
#define VECTOR2D_H

#include <iostream>
#include <cmath>
#include "Point.h"

class Vector2D {
public:
    float X, Y;

public:
    Vector2D(float x = 0, float y = 0) : X(x), Y(y) {}

    Vector2D(const Point& p) : X(p.X), Y(p.Y) {}

    Vector2D Normalized() const {
        float length = sqrt(X * X + Y * Y);
        if (length > 0) {
            return Vector2D(X / length, Y / length);
        }
        std::cerr << "Warning: Normalizing a zero-length vector" << std::endl;
        return Vector2D(0, 0);
    }

    float Length() const {
        return sqrt(X * X + Y * Y);
    }

    inline Vector2D operator/(const float scalar) const {
        if (scalar != 0)
            return Vector2D(X / scalar, Y / scalar);
        else {
            std::cerr << "Warning: Division by zero in Vector2D" << std::endl;
            return Vector2D(0, 0);
        }
    }

    static float Distance(const Vector2D& a, const Vector2D& b) {
        float dx = a.X - b.X;
        float dy = a.Y - b.Y;
        return sqrt(dx * dx + dy * dy);
    }

    inline Vector2D operator+(const Vector2D& v2) const {
        return Vector2D(X + v2.X, Y + v2.Y);
    }


    inline Vector2D operator-(const Vector2D& v2) const {
        return Vector2D(X - v2.X, Y - v2.Y);
    }

    inline Vector2D operator*(const float scalar) const {
        return Vector2D(X * scalar, Y * scalar);
    }

    void Log(std::string msg = "") const {
        std::cout << msg << " (X, Y) = (" << X << ", " << Y << ")" << std::endl;
    }
};

#endif // VECTOR2D_H
