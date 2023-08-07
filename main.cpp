#include <iostream>
#include <filesystem>
#include <string_view>
#include <vector>

#include "parser.h"
#include "converter.h"

namespace fs = std::filesystem;

int main(int argc, char* argv[]) {

    if (argc == 1) {
        std::cerr << "Usage: stlEdit [options...]\n"
                  << "Try 'stlEdit -h' for more information.\n";
        return 1;
    }

    std::vector<std::string_view> args(argv + 1, argv + argc);

    if (argc == 2 && (args[0] == "-h" || args[0] == "--help")) {
        std::cout << "Usage: stlEdit [options...]\n\n"
                  << "Options:\n"
                  << "\t-ascii \t\tConvert *.stl file to ascii format.\n"
                  << "\t-binary\t\tConvert *.stl file to binary format.\n"
                  << "\t-file \t\tSpecify the *.stl file location.\n"
                  << "\t-h, --help \tDisplay help message.\n"
                  << "\t-merge \t\tMerge transferred *.stl files into one ascii file.\n"
                  << "\t-mv \t\tMove object. All three movement size must be specified.\n"
                  << "\t\t\tTo specify the movement axis is typing '-mvx' or '-mvyz'.\n"
                  << "\t-r \t\tRotate object. All three angles must be specified.\n"
                  << "\t\t\tTo specify the rotate axis and rotate order is typing '-rx' or '-ryz'.\n"
                  << "\t\t\tThe order of axis names is important.\n"
                  << "\t-s \t\tScale object. To specify the scale axis is typing '-sx' or '-syz'.\n"
                  << "\t-tp \t\tCreate topoSetDict and refineMeshDict files in the system folder.\n\n";
        return 0;
    }
    
    if(parser::has_option(args, "-ascii")) {

    }

}
