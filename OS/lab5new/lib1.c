//
// Created by aldes on 28.11.2021.
//

#include "lib1.h"
#include <math.h>

double integrateSin(double leftBound, double rightBound, double step) {
    double sinNumeric = 0;

    double left = leftBound;
    double right = left + step;
    double point = (left + right) / 2;
    while (right <= rightBound) {
        sinNumeric += (sin(point) * step);
        left = right;
        right += step;
        point = (left + right) / 2;
    }

    return sinNumeric;
}

double calculatePI(int seriesLength) {
    double PI = 0;

    for (int i = 0; i < seriesLength - 1; i++) {
        PI += pow(-1, i) / (2 * i + 1);
    }

    return 4 * PI;
}