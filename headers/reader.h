#ifndef READER_H
#define READER_H

#include <iostream>
#include <exception>
#include <filesystem>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <string>
#include <utility>

#include "parser.h"
#include "stl.h"
#include <tuple>

namespace fs = std::filesystem;

std::tuple<STL, fs::path, bool> read(const Parser&);
fs::path find_file();
void ascii_read(const fs::path&, STL&);
void binary_read(const fs::path&, STL&);
bool is_binary(const fs::path&);
void check_location_and_copy(const fs::path&);

#endif
