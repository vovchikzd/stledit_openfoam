#ifndef PARSER_H
#define PARSER_H

#include <string_view>
#include <vector>

namespace parser {

    std::vector<std::string_view> get_options(
            const std::vector<std::string_view>&,
            const std::string_view&,
            size_t options_number = 1);

    bool has_option(
            const std::vector<std::string_view>& args,
            const std::string_view& option_name);

    std::string_view get_suboptions(
        const std::vector<std::string_view>&,
        const std::string_view&);
}

#endif
