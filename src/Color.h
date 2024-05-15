//
// Created by joachimverleysen on 5/5/24.
//

#ifndef MYENGINE_COLOR_H
#define MYENGINE_COLOR_H


class Color {
public:
    Color() : red(0), green(0), blue(0) {}

    Color(double r, double g, double b) : red(r * 255), green(g * 255), blue(b * 255) {};
    double red;
    double green;
    double blue;

    Color operator+(const Color &other) const;

    Color operator+=(const Color &rhs) const;
};

#endif //MYENGINE_COLOR_H
