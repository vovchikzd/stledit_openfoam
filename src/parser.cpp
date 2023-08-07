#include "parser.h"

std::vector<std::string_view> parser::get_options(
            const std::vector<std::string_view>& args,
            const std::string_view& option_name,
            size_t options_number) {
    std::vector<std::string_view> options;
    for (auto it = args.begin(), end = args.end(); it != end; ++it) {
        if (*it == option_name && it + 1 != end) {
            for(size_t i = 0; i < options_number; ++i) {
                options.push_back(*(++it));
            }
        }
    }

    return options;
}

bool parser::has_option(
            const std::vector<std::string_view>& args,
            const std::string_view& option_name) {
    for(auto it = args.begin(), end = args.end(); it != end; ++it) {
        if (*it == option_name ||
                (it->find(option_name) != std::string_view::npos)) {
            return true;
        }
    }

    return false;
}

std::string_view parser::get_suboptions(
        const std::vector<std::string_view>& args,
        const std::string_view& option_name) {
    for (auto it = args.begin(), end = args.end(); it != end; ++it) {
        if (it->find(option_name) != std::string_view::npos) {
            std::string_view result(it->begin() + 2, it->end());
           return result;
        }
    }
    return "";
}
