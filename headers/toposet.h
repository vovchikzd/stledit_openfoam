#ifndef TOPOSET_H
#define TOPOSET_H

#include <algorithm>
#include <vector>
#include <utility>
#include <ios> // std::fixed
#include <fstream>
#include <filesystem>
#include <string>
#include <sstream>
#include <cmath>

namespace fs = std::filesystem;

#include "stl.h"

template <typename T>
std::string to_string(T number) {
    std::ostringstream string_stream;
    string_stream << number;
    std::string result = string_stream.str();
    return result;
}

void toposet(const STL&, const int&);
std::vector<std::pair<float, float>> facet_minmax(const Facet&);
void write_toposet(const std::vector<std::pair<float, float>>&, const int&);

#endif
