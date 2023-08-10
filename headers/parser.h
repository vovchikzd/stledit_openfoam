#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <filesystem>
#include <string>
#include <exception>

namespace fs = std::filesystem;

struct Parser {

    bool is_to_ascii = false;
    bool is_to_binary = false;

    bool is_rotate = false;
    std::vector<double> rotate_angles;
    std::string rotate_order;

    bool is_move = false;
    std::vector<double> move_lengths;
    std::string move_axis;

    bool is_scale = false;
    std::vector<double> scale_values;
    std::string scale_axis;

    bool is_toposet = false;

    bool is_merge = false;
    std::vector<fs::path> merge_files;

    bool is_diff_location = false;
    fs::path specify_file;

    fs::path out_name = "out.stl";

    
    Parser() = default;
    Parser(std::vector<std::string>&);
    Parser(int, char**);
    void parse(std::vector<std::string>&);
    
}; // struct parser

#endif
