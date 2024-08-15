//
// Created by joachimverleysen on 2/27/24.
//

#ifndef MYENGINE_MYLSYSTEM2D_H
#define MYENGINE_MYLSYSTEM2D_H

#include "LineDrawer.h"

#include <string>
#include <vector>
#include <set>
#include <cmath>
#include <map>
#include <fstream>
#include <algorithm>

class Color;

class MyLSystem2D {
    std::vector<Point2D> points;

private:
    std::string inputfile;
    std::set<char> alphabet;
    double Angle;
    std::string initiator;
    double starting_angle;
    std::string finalString;    // Result std::string after the rules rules have been applied (i times)
    std::map<char, int> drawFunction;
    Lines2D lines;
    std::map<char, std::string> rules;
    int size;
    Color color;
    Color bgColor;
    unsigned int iterations;

public:
    MyLSystem2D(std::string inputfile) : inputfile(inputfile) {parser(inputfile);}

    int parser(std::string inputfile);

    void computePoints(std::vector<Point2D> &points);

    void _str2Points(const std::string &str, std::vector<Point2D> &points);

    Point2D _computeNextPoint(Point2D &p1, double angle);

    void setAlphabet(const std::set<char> &alphabet);

    void setAngle(double angle);

    void setInitiator(const std::string &initiator);

    void setStartingAngle(double startingAngle);

    void setPoints(const std::vector<Point2D> &points);

    void applyReplacement(std::string &s);

    std::vector<Point2D> const getPoints();

    const std::string &getInputfile() const;

    void setInputfile(const std::string &inputfile);

    const std::set<char> &getAlphabet() const;

    double getAngle() const;

    const std::string &getInitiator() const;

    double getStartingAngle() const;

    const std::string &getFinalString() const;

    void setFinalString(const std::string &finalString);

    const std::map<char, int> &getDrawFunction() const;

    void setDrawFunction(const std::map<char, int> &drawFunction);

    const Lines2D &getLines() const;

    void setLines(const Lines2D &lines);

    const std::map<char, std::string> &getRules() const;

    void setRules(const std::map<char, std::string> &rules);

    int getSize() const;

    void setSize(int size);

    const Color &getColor() const;

    void setColor(const Color &color);

    const Color &getBgColor() const;

    void setBgColor(const Color &bgColor);

    unsigned int getIterations() const;

    void setIterations(unsigned int iterations);
};



#endif //MYENGINE_MYLSYSTEM2D_H
