#ifndef MERGE_H
#define MERGE_H

#include <filesystem>
#include <fstream>
#include <string>
#include <ios>

#include "parser.h"
#include "reader.h"

namespace fs = std::filesystem;

void merge(const Parser&);
void binary_merge(const fs::path&, std::ofstream&);
void ascii_merge(const fs::path&, std::ofstream&);

#endif
