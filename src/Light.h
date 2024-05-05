//
// Created by joachimverleysen on 5/5/24.
//

#ifndef MYENGINE_LIGHT_H
#define MYENGINE_LIGHT_H


#include "LineDrawer.h"

class Light {
public:
    Color ambientLight;
    Color diffuseLight;
    Color specularLight;
};

class PointLight: public Light {
public:
    // Point source location
    Vector3D location;

    // Spotlight angle
    double spotAngle;
};

class InfLight: public Light {
public:
    // Light direction
    Vector3D ldVector;
};

typedef std::list<Light> Lights3D;


#endif //MYENGINE_LIGHT_H
