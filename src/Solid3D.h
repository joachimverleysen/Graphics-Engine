//
// Created by joachimverleysen on 3/27/24.
//

#ifndef MYENGINE_SOLID3D_H
#define MYENGINE_SOLID3D_H


#include "Figure.h"

/**
 * This class generates platonic solids.
 * \n All functions take a Figure object as parameter by reference
 * \n The points and faces of the figure will be set accordingly
 */

class Solid3D {
private:

public:
    void generateCube(Figure& fig);

    void generateTetrahedron(Figure& fig);

    void generateOctahedron(Figure& fig);

    void generateIcosahedron(Figure& fig);

    void generateDodecahedron(Figure& fig);
};


#endif //MYENGINE_SOLID3D_H
