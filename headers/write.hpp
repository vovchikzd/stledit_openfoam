#ifndef WRITE_H
#define WRITE_H

#include <filesystem>
#include <fstream>
#include <vector>
namespace fs = std::filesystem;

#include "stl.hpp"

void write(const std::vector<STL>&, const fs::path&, bool);
void write_ascii(const std::vector<STL>&, const fs::path&);
void binary_ascii(const std::vector<STL>&, const fs::path&);

#endif
