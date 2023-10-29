#ifndef READ_H
#define READ_H

#include "stl.hpp"
#include "tools.hpp"

void binary_read(const fs::path&, STL&);
void ascii_read(const fs::path&, STL&);
std::vector<STL> read(const std::vector<fs::path>&);

#endif
