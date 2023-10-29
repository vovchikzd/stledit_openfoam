#ifndef TOOLS_H
#define TOOLS_H

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "stl.hpp"

namespace fs = std::filesystem;

std::vector<fs::path> find_file();
void check_location_and_copy(const fs::path&, const fs::path&);
bool is_binary(const fs::path&);
fs::path get_write_file(const std::vector<fs::path>&, const fs::path&);
std::vector<std::pair<float, float>> facet_minmax(const Facet&);

#endif
