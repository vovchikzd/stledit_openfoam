#ifndef WAVE_H
#define WAVE_H

#include <vector>
#include <utility>
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

#include "stl.h"
#include "tools.h"

void create_wave_stl(const std::vector<STL>&);
void write_wave_file(const std::vector<std::pair<int, int>>&);

#endif
