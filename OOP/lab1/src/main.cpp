//
// Created by djent on 07.09.2021.
//

#include "../headers/figures.h"

int main() {
    Rhombus a(std::cin);
    a.print();
    std::cout << "Vertexes number = " << a.VertexesNumber() << '\n';
    std::cout << "Square = " << a.square() << '\n';

    Trapeze b(std::cin);
    b.print();
    std::cout << "Vertexes number = " << b.VertexesNumber() << '\n';
    std::cout << "Square = " << b.square() << '\n';

    Pentagon c(std::cin);
    c.print();
    std::cout << "Vertexes number = " << c.VertexesNumber() << '\n';
    std::cout << "Square = " << c.square() << '\n';
}