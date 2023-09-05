#ifndef WRITE_H
#define WRITE_H

#include <vector>
#include <fstream>
#include <filesystem>
namespace fs = std::filesystem;

#include "stl.h"

void write(const std::vector<STL>&, const fs::path&, bool);
void write_ascii(const std::vector<STL>&, const fs::path&);
void binary_ascii(const std::vector<STL>&, const fs::path&);

#endif
