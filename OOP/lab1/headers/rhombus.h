//
// Created by djent on 09.09.2021.
//

#ifndef LAB1_RHOMBUS_H
#define LAB1_RHOMBUS_H

#include <iostream>

#include "figure.h"

class Rhombus : Figure {
public:
    explicit Rhombus(std::istream &in);
    void print() override;
    double square() override;
    int VertexesNumber() override;
private:
    Point a, b, c, d;
    double sqr;
    bool isValid();

};
#endif //LAB1_RHOMBUS_H
