//
// Created by joachimverleysen on 3/27/24.
//

#ifndef MYENGINE_MYPARSER_H
#define MYENGINE_MYPARSER_H


#include "../tools/ini_configuration.h"
#include "Drawing3D.h"
#include "LSystem.h"

/**
 * This class parses ini files
 */
class MyParser {

public:
    void drawing3D_parse(const ini::Configuration &conf, Drawing3D &drawing);

    LSystem lsys_parse(const ini::Configuration &conf);
};


#endif //MYENGINE_MYPARSER_H
