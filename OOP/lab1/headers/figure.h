//
// Created by djent on 07.09.2021.
//

#ifndef LAB1_FIGURE_H
#define LAB1_FIGURE_H

#include <cmath>

#include "point.h"

class Figure {

protected:
    virtual void print() = 0;
    virtual double square() = 0;
    virtual int VertexesNumber() = 0;
    bool valid;

};

#endif //LAB1_FIGURE_H
