#ifndef ASSIGNMENT_FRAMEWORK_SHAPES2D_H
#define ASSIGNMENT_FRAMEWORK_SHAPES2D_H

#include "BasicShapes.h"

class Line : public BasicShape {
public:
    Line();
};

class Circle : public BasicShape {
public:
    Circle();

public:
    void naive_circle(int segment);

    void as_triangle();

    int segments = 100;
};

class Rectangle : public BasicShape {
public:
    Rectangle();

public:
    void as_triangle();
};

#endif //ASSIGNMENT_FRAMEWORK_SHAPES2D_H
