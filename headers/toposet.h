#ifndef TOPOSET_H
#define TOPOSET_H

#include <cmath>
#include <fstream>
#include <vector>
#include <utility>
#include <string>
#include <sstream>
#include <algorithm>
#include <filesystem>
namespace fs = std::filesystem;

#include "stl.h"

template <typename T>
std::string to_string(T number) {
    std::ostringstream string_stream;
    string_stream << number;
    std::string result = string_stream.str();
    return result;
}

void create_toposet(const std::vector<STL>&, const size_t&);
void write_toposet(const std::vector<std::pair<float, float>>&, const size_t&);
std::vector<std::pair<float, float>> facet_minmax(const Facet&);

#endif
