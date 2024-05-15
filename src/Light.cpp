//
// Created by joachimverleysen on 5/5/24.
//

#include "Light.h"

bool Light::diffuse() const {
    if (direction.x == 0 && direction.y==0 && direction.z==0) return false;
    return true;
}
