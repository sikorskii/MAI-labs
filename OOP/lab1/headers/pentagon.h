//
// Created by djent on 09.09.2021.
//

#ifndef LAB1_PENTAGON_H
#define LAB1_PENTAGON_H

#include <iostream>

#include "figure.h"

class Pentagon : Figure {
public:
    explicit Pentagon(std::istream &in);
    void print() override;
    double square() override;
    int VertexesNumber() override;
private:
    Point a, b, c, d, e;
    double sqr;
    bool isValid();
};

#endif //LAB1_PENTAGON_H
