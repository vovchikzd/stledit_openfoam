#include "parser.hpp"

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

      int i = 0;
      ++it;
      while (i < 3 && it != end &&
             ((*it)[0] != '-' || !std::isalpha((*it)[1]))) {
        rotate_angles.push_back(std::stof(*it));
        ++i;
        ++it;
      }
      --it;

    } else if (it->find("-mv") != std::string::npos) {
      is_move = true;

      for (auto start = it->begin() + 3, back = it->end(); start != back;
           ++start) {
        move_axes += *start;
      }

      int i = 0;
      ++it;
      while (i < 3 && it != end &&
             ((*it)[0] != '-' || !std::isalpha((*it)[1]))) {
        move_lengths.push_back(std::stof(*it));
        ++i;
        ++it;
      }
      --it;

    } else if (it->find("-s") != std::string::npos) {
      is_scale = true;

      for (auto start = it->begin() + 2, back = it->end(); start != back;
           ++start) {
        scale_axes += *start;
      }

      int i = 0;
      ++it;
      while (i < 3 && it != end &&
             ((*it)[0] != '-' || !std::isalpha((*it)[1]))) {
        scale_values.push_back(std::stof(*it));
        ++i;
        ++it;
      }
      --it;

    } else if (*it == "-tp") {
      is_toposet = true;
      if (it + 1 != end && (*(it + 1))[0] != '-') {
        file_numbers = std::stoull(*(++it));
      }
    } else if (*it == "-wv") {
      is_wave = true;
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
      std::cerr << "Error! Incorrect use of the \"" << *it << "\" argument.\n";
      throw std::invalid_argument("Invalid argument");

    } else {
      std::cerr << "Error! Unknown argument: \"" << *it << "\".\n";
      throw std::invalid_argument("Invalid argument");
    }
  }
}

Parser::Parser(std::vector<std::string>& args) { parse(args); }

Parser::Parser(int argc, char* argv[]) {
  std::vector<std::string> args(argv + 1, argv + argc);
  parse(args);
}
