//
// Created by joachimverleysen on 3/27/24.
//

#ifndef MYENGINE_MYPARSER_H
#define MYENGINE_MYPARSER_H


#include "../tools/ini_configuration.h"
#include "Drawing3D.h"
#include "MyLSystem2D.h"
#include "tools/ini_configuration.h"

/**
 * This class parses ini
 */
class MyParser {

public:
    void drawing3D_parse(const ini::Configuration &conf, Drawing3D &drawing);

    MyLSystem2D lsys_parse(const ini::Configuration &conf);

    void parse_lights(const ini::Configuration &conf, Drawing3D &drawing);

    void parse_figure(Figure &figure, const ini::Configuration &conf, Drawing3D &drawing, const int i);

    void parse_solid3d(Figure &figure, const ini::Configuration &conf, Drawing3D &drawing);
};


#endif //MYENGINE_MYPARSER_H
