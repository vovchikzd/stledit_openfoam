#ifndef WAVE_H
#define WAVE_H

#include <filesystem>
#include <fstream>
#include <utility>
#include <vector>

namespace fs = std::filesystem;

#include "stl.hpp"
#include "tools.hpp"

void create_wave_stl(const std::vector<STL>&);
void write_wave_file(const std::vector<std::pair<int, int>>&);

#endif
