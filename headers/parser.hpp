#ifndef PARSER_H
#define PARSER_H

#include <exception>
#include <filesystem>
#include <iostream>
#include <locale>
#include <string>
#include <vector>

namespace fs = std::filesystem;

struct Parser {
  bool is_to_ascii = false;
  bool is_to_binary = false;

  bool is_rotate = false;
  std::vector<float> rotate_angles;
  std::string rotate_order;

  bool is_move = false;
  std::vector<float> move_lengths;
  std::string move_axes;

  bool is_scale = false;
  std::vector<float> scale_values;
  std::string scale_axes;

  bool is_toposet = false;
  size_t file_numbers = 3;

  bool is_wave = false;

  bool is_merge = false;
  std::vector<fs::path> merge_files;

  bool is_diff_location = false;
  fs::path specify_file;

  fs::path out_name;

  Parser() = default;
  Parser(std::vector<std::string>&);
  Parser(int, char**);
  void parse(std::vector<std::string>&);

};  // struct parser

#endif
