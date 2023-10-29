#include <iostream>
#include <vector>
#include <exception>
#include <filesystem>

#include "parser.hpp"
#include "stl.hpp"
#include "tools.hpp"
#include "read.hpp"
#include "edit.hpp"
#include "toposet.hpp"
#include "write.hpp"
#include "wave.hpp"

namespace fs = std::filesystem;

int main(int argc, char* argv[]) {

    std::string help = "Usage: stlEdit [options...]\n\n"
                       "Options:\n"
                       "\t-ascii \t\tConvert *.stl file to ascii format.\n"
                       "\t-binary\t\tConvert *.stl file to binary format.\n"
                       "\t-file \t\tSpecify the *.stl file location.\n"
                       "\t-h, --help \tDisplay help message.\n"
                       "\t-merge \t\tMerge transferred *.stl files into one file.\n"
                       "\t-mv \t\tMove object. To specify the movement axis is typing '-mvx' or '-mvyz'.\n"
                       "\t-o \t\tSpecifies the name of the resulting *.stl file.\n"
                       "\t-r \t\tRotate object. To specify the rotate axis and rotate order\n"
                       "\t\t\tis typing '-rx' or '-ryz'. The order of axis names is important.\n"
                       "\t-s \t\tScale object. By default is requires one scale value.\n"
                       "\t\t\tTo specify the scale axis is typing '-sx' or '-syz'.\n"
                       "\t-tp \t\tCreate topoSetDict and refineMeshDict files in the system folder.\n"
                       "\t-wv \t\tCreate a wave *.stl file based on model.\n\n";
					   
    if (argc == 1) {
        std::cerr << help;
        return 1;
    }

    std::vector<std::string> args(argv + 1, argv + argc);

    if (argc == 2 && (args[0] == "-h" || args[0] == "--help")) {
        std::cout << help;

        return 0;
    }

    Parser parser;

    try {
        parser = Parser(args);
    } catch (std::exception&) {
        std::cerr << help;
        return 1;
    }

    fs::path new_name = parser.out_name;

    std::vector<fs::path> files;
    if (!parser.is_diff_location && !parser.is_merge) {

        try {
            files = find_file();
        } catch (std::exception&) {
            std::cerr << help;
            return 1;
        }

    } else if (parser.is_diff_location) {
        files.push_back(parser.specify_file);
    } else if (parser.is_merge) {
        files = parser.merge_files;
    }

    bool is_edit = (parser.is_rotate || parser.is_move || parser.is_scale);
    bool is_convert = (parser.is_to_binary || parser.is_to_ascii);

    std::vector<STL> objects;
    if (!is_edit && !is_convert && !parser.is_toposet
        && !parser.is_merge && !parser.is_wave) {
        check_location_and_copy(files[0], new_name);
        return 0;
    } else {
        objects = read(files);
    }

    if (is_edit) {
        edit(parser, objects);
    }

    if (parser.is_toposet) {
        create_toposet(objects, parser.file_numbers);
    }

    if (parser.is_wave) {
        create_wave_stl(objects);
    }

    fs::path write_file = get_write_file(files, new_name);

    bool binary_code = true;
    if (!is_convert && files.size() > 1) {
        binary_code = false;
    } else if (!is_convert && files.size() == 1) {
        binary_code = is_binary(files[0]);
    } else if (is_convert) {
        if (parser.is_to_binary)
            binary_code = true;
        else if (parser.is_to_ascii)
            binary_code = false;
    }
    
    write(objects, write_file, binary_code);

    return 0;
}
