#include <iostream>
#include <vector>
#include <exception>
#include <filesystem>

#include "parser.h"

namespace fs = std::filesystem;

int main(int argc, char* argv[]) {

    std::string help = "Usage: stlEdit [options...]\n\n"
                       "Options:\n"
                       "\t-ascii \t\tConvert *.stl file to ascii format.\n"
                       "\t-binary\t\tConvert *.stl file to binary format.\n"
                       "\t-file \t\tSpecify the *.stl file location.\n"
                       "\t-h, --help \tDisplay help message.\n"
                       "\t-merge \t\tMerge transferred *.stl files into one ascii file.\n"
                       "\t-mv \t\tMove object. To specify the movement axis is typing '-mvx' or '-mvyz'.\n"
                       "\t-o \t\tSpecifies the name of the resulting *.stl file.\n"
                       "\t-r \t\tRotate object. To specify the rotate axis and rotate order\n"
                       "\t\t\tis typing '-rx' or '-ryz'. The order of axis names is important.\n"
                       "\t-s \t\tScale object. By default is requires one scale value.\n"
                       "\t\t\tTo specify the scale axis is typing '-sx' or '-syz'.\n"
                       "\t-tp \t\tCreate topoSetDict and refineMeshDict files in the system folder.\n\n";

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


    return 0;
}
