//
// Created by djent on 07.09.2021.
//

#ifndef LAB1_TRAPEZE_H
#define LAB1_TRAPEZE_H

#include <iostream>

#include "figure.h"

class Trapeze : Figure {

public:
    explicit Trapeze(std::istream &in);
    void print() override;
    double square() override;
    int VertexesNumber() override;

private:
    Point a, b, c, d;
    double sqr;
    bool isValid();
};

#endif //LAB1_TRAPEZE_H