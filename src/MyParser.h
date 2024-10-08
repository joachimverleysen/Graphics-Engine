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

enum SuccessEnum {success, partial, failure};


class MyParser {

public:
    static SuccessEnum drawing3D_parse(const ini::Configuration &conf, Drawing3D &drawing);

    static MyLSystem2D parse_Lsystem2D(const ini::Configuration &conf);

    static void parse_lights(const ini::Configuration &conf, Drawing3D &drawing);

    static void parse_figure(Figure &figure, const ini::Configuration &conf, Drawing3D &drawing, const int i);

    static void parse_solid3d(Figure &figure, const ini::Configuration &conf, Drawing3D &drawing);
};


#endif //MYENGINE_MYPARSER_H
