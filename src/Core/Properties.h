#ifndef PROPERTIES_H   // Nếu chưa được định nghĩa, tiến hành định nghĩa
#define PROPERTIES_H   // Định nghĩa PROPERTIES_H để ngăn chặn việc bao gồm lại file này

#include <string>
#include "Vector2D.h"

class Properties {
public:
    Vector2D position;  // Vị trí của đối tượng
    std::string textureID;  // ID của texture

    // Constructor
    Properties(Vector2D pos, const std::string& texture);

    // Destructor (nếu cần)
    ~Properties() {}
};

#endif // PROPERTIES_H
