//
// Created by joachimverleysen on 5/5/24.
//

#ifndef MYENGINE_LIGHT_H
#define MYENGINE_LIGHT_H


#include "Color.h"
#include "../tools/vector3d.h"
#include <list>
#include <vector>


class Light {
public:
Color ambientLight;
Color diffuseLight;
Vector3D direction;
bool infinity = true;

bool diffuse() const;

};

using Lights3D = std::list<Light>;


#endif //MYENGINE_LIGHT_H
