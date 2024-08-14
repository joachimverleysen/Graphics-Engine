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

MyLSystem2D MyParser::parse_Lsystem2D(const ini::Configuration &conf) {
    std::string type = conf["General"]["type"].as_string_or_die();
    int size = conf["General"]["size"].as_int_or_die();

    vector<double> bgColor = conf["General"]["backgroundcolor"].as_double_tuple_or_die();
    string inputfile = conf["2DLSystem"]["inputfile"].as_string_or_die();
    vector<double> color = conf["2DLSystem"]["color"].as_double_tuple_or_die();
    MyLSystem2D result(inputfile);
    result.setSize(size);
    result.setColor(Color(*(color.begin()), *(color.begin()+1), *(color.begin()+2)));
    result.setBgColor(Color(*(bgColor.begin()), *(bgColor.begin()+1), *(bgColor.begin()+2)));
    return result;
}
