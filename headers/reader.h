#ifndef READER_H
#define READER_H

#include <iostream>
#include <exception>
#include <filesystem>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <string>

#include "parser.h"
#include "stl.h"

namespace fs = std::filesystem;

STL read(const Parser&);
fs::path find_file();
void ascii_read(const fs::path&, STL&); // todo
void binary_read(const fs::path&, STL&); // todo
bool is_binary(const fs::path&); // todo
void check_location_and_copy(const fs::path&);

#endif
