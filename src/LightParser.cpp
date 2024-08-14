//
// Created by joachimverleysen on 8/14/24.
//
#include "MyParser.h"
#include "tools/ini_configuration.h"
#include "Drawing3D.h"
#include "LineDrawer.h"
#include "Solid3D.h"
#include "MyLSystem2D.h"
#include "MyLSystem3D.h"
#include "Light.h"



void MyParser::parse_lights(const ini::Configuration &conf, Drawing3D &drawing) {
    Lights3D lights;
    for (int i=0; i!=drawing.getNrLights(); i++) {
        Light light;

        string lightname = "Light" + to_string(i);
        vector<double> ambient = conf[lightname]["ambientLight"].as_double_tuple_or_default({0, 0, 0});
        vector<double> diffuse = conf[lightname]["diffuseLight"].as_double_tuple_or_default({0, 0, 0});
//        vector<double> specular = conf[lightname]["specular"].as_double_tuple_or_default({0, 0, 0});

        vector<double> direction_ = conf[lightname]["direction"].as_double_tuple_or_default({0, 0, 0});
        Vector3D direction;
        direction.x = direction_[0];
        direction.y = direction_[1];
        direction.z = direction_[2];
        bool infty = conf[lightname]["infinity"].as_bool_or_default(false);


        Color ambLight(ambient[0], ambient[1], ambient[2]);
        Color diffLight(diffuse[0], diffuse[1], diffuse[2]);
        light.ambientLight = ambLight;
        light.diffuseLight = diffLight;

        light.infinity = infty;
        light.direction = direction;




        lights.push_back(light);
    }
    drawing.setLights(lights);
}