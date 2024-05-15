//
// Created by joachimverleysen on 5/5/24.
//

#include "Color.h"

Color Color::operator+(const Color &other) const {
    return Color((this->red/255) + other.red / 255, (this->green / 255) + other.green / 255, (this->blue / 255) + other.blue / 255);

}

Color Color::operator+=(const Color &rhs) const {
    return Color((this->red/255)+rhs.red/255, (this->green/255)+rhs.green/255, (this->blue/255)+rhs.blue/255);

}
