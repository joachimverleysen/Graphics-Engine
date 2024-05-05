//
// Created by joachimverleysen on 5/5/24.
//

#ifndef MYENGINE_LIGHT_H
#define MYENGINE_LIGHT_H


#include "Color.h"
#include <list>

class Light {
public:
Color ambientLight;
};

using Lights3D = std::list<Light>;


#endif //MYENGINE_LIGHT_H
