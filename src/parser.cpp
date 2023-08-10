#include "parser.h"

void Parser::parse(std::vector<std::string>& options) {
    for (auto it = options.begin(), end = options.end(); it != end; ++it) {
        
        if (*it == "-ascii") {
            is_to_ascii = true;

        } else if (*it == "-binary") {
            is_to_binary = true;

        } else if (it->find("-r") != std::string::npos) {

            is_rotate = true;

            for (auto start = it->begin() + 2, back = it->end(); start != back;
                    ++start) {
                rotate_order += *start;
            }


            auto steps = (rotate_order.empty() ? 3 : rotate_order.size());

            for (size_t i = 0; i < steps; ++i) {
                rotate_angles.push_back(std::stod(*(++it)));
            }

        } else if (it->find("-mv") != std::string::npos) {

            is_move = true;
 
            for (auto start = it->begin() + 3, back = it->end(); start != back;
                    ++start) {
                move_axis += *start;
            }

            auto steps = (move_axis.empty() ? 3 : move_axis.size());

            for (size_t i = 0; i < steps; ++i) {
                move_lengths.push_back(std::stod(*(++it)));
            }

        } else if (it->find("-s") != std::string::npos) {

            is_scale = true;

            for (auto start = it->begin() + 2, back = it->end(); start != back;
                    ++start) {
                scale_axis += *start;
            }

            auto steps = (scale_axis.empty() ? 1 : scale_axis.size());

            for (size_t i = 0; i < steps; ++i) {
                scale_values.push_back(std::stod(*(++it)));
            }

        } else if (*it == "-tp") {
            is_toposet = true;
            
        } else if (*it == "-merge") {
            is_merge = true;
            ++it;
            while (it != end && (*it)[0] != '-') {
                merge_files.push_back(*(it));
                ++it;
            }
            --it;

        } else if (*it == "-file") {
            is_diff_location = true;
            specify_file = *(++it);

        } else if (*it == "-o") {
            out_name = *(++it);

        } else if (*it == "-h" || *it == "--help") {
            throw std::runtime_error(std::string(*it));
        } else {
            throw std::invalid_argument(std::string(*it));
        }
    }
}

Parser::Parser(std::vector<std::string>& args) {
    parse(args);
}

Parser::Parser(int argc, char* argv[]) {
    std::vector<std::string> args(argv + 1, argv + argc);
    parse(args);
}
