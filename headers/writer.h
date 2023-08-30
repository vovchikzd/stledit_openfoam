#ifndef WRITER_H
#define WRITER_H

#include <filesystem>
#include <fstream>
#include <string>

// #include "reader.h"
#include "stl.h"
#include "parser.h"

void write(const STL&, const fs::path&, bool, const Parser&);
void write_binary(const STL&, std::ofstream&);
void write_ascii(const STL&, std::ofstream&, const std::string&);
void check_location();
fs::path check_file(const fs::path&, const std::string&);

#endif
