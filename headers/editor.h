#ifndef EDITOR_H
#define EDITOR_H

#include <string>
#include <cmath>

#include "stl.h"
#include "matrix.h"

template <typename T>
T rad(T angle) {
    return angle * (3.14 / 180);
}

void axis_move(Facet&, const int&, const float&);
void get_axis_and_move(Facet&, const char&, const float&);
void move(Facet&, const std::string&, const std::vector<float>&);

void axis_scale(Facet&, const int&, const float&);
void get_axis_and_scale(Facet&, const char&, const float&);
void scale(Facet&, const std::string&, const std::vector<float>&);

void axis_rotate(Facet&, Matrix<float, 3, 3>&);
void get_axis_and_rotate(Facet&, const char&, const float&);
void rotate(Facet&, const std::string&, const std::vector<float>&);
#endif
