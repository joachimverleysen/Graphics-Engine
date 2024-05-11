//
// Created by joachimverleysen on 3/31/24.
//

#ifndef MYENGINE_ZBUFFER_H
#define MYENGINE_ZBUFFER_H
#include <vector>
#include <limits>

class ZBuffer: public std::vector<std::vector<double>> {
public:
    int width;
    int height;

public:
    ZBuffer(const int width, const int height) : width(width), height(height) {
        for (int i=0; i<width; i++) {
            vector<double> row;
            for (int j=0; j<height; j++) {
                double infty = std::numeric_limits<double>::infinity();
                row.push_back(infty);
            }
            this->push_back(row);
        }
    };

};


#endif //MYENGINE_ZBUFFER_H
