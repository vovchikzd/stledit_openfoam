#include "../headers/reader.h"

fs::path find_file() {
    fs::path constant("constant");
    fs::path triSurface("triSurface");
    fs::path result;

    if(!fs::is_directory(constant) || !fs::exists(constant)) {
        std::cerr << "The default file location doesn't exist.\n"
                  << "Specify file location.\n";
        throw std::exception();
    } else if (!fs::is_directory(constant/triSurface) ||
               !fs::exists(constant/triSurface)) {
        std::cerr << "The default file location doesn't exist.\n"
                  << "Specify file location.\n";
        throw std::exception();
    }

    if (fs::is_empty(constant/triSurface)) {
        std::cerr << "The default file location is empty.\n"
                  << "Specify file location.\n";
        throw std::exception();
    }

    fs::directory_iterator default_location(constant/triSurface);
    std::vector<fs::directory_entry> files;

    for (auto file: default_location) {
        if (file.path().extension() == ".stl") {
            files.push_back(file);
        }
    }

    if (files.empty()) {
        std::cerr << "There isn't .stl file in the default location.\n"
                  << "Specify file location\n";
        throw std::exception();
    } else if (files.size() == 1) {
        result = files[0].path();
    } else {
        std::cout << "There are some *.stl files.\n"
                  << "Choose one or cancel and specify location\n\n"
                  << "Files:\n";
        for (size_t i = 0; i < files.size(); ++i) {
            std::cout << i + 1 << ". " << files[i].path()
                                                  .filename()
                                                  .string() << '\n';
        }
        std::cout << "\nType a number (0 or Ctrl-c to cancel): ";
        size_t choice;
        std::cin >> choice;

        if (!choice) { std::exit(0); }
        else { result = files[choice - 1].path(); }
    }

    return result;
}

void check_location_and_copy(const fs::path& file) {
    fs::path constant("constant");
    fs::path triSurface("triSurface");

    fs::path file_parent = file.parent_path();

    if (!(file_parent == (constant/triSurface))) {
        if (!fs::is_directory(constant) || !fs::exists(constant)) {
            fs::create_directory(constant);
        }
        if (!fs::is_directory(constant/triSurface) ||
            !fs::exists(constant/triSurface)) {
            fs::create_directory(constant/triSurface);
        }

        fs::copy(file, constant/triSurface);
    }
}

bool is_binary(const fs::path& file) {
    std::ifstream input_stl(file);

    std::string first_word;
    input_stl >> first_word;
    input_stl.close();

    if (first_word == "solid") {
        return false;
    } else {
        return true;
    }
}

void binary_read(const fs::path& file, STL& object) {
    std::ifstream input_stl(file);
    uint32_t facets_number;

    input_stl.read(reinterpret_cast<char*>(&object.header), 80);
    input_stl.read(reinterpret_cast<char*>(&facets_number), 4);

    for (uint32_t i = 0; i < facets_number; ++i) {
        Facet facet;
        input_stl.read(reinterpret_cast<char*>(&facet.normal), 12);
        input_stl.read(reinterpret_cast<char*>(&facet.first_vertex), 12);
        input_stl.read(reinterpret_cast<char*>(&facet.second_vertex), 12);
        input_stl.read(reinterpret_cast<char*>(&facet.third_vertex), 12);
        input_stl.read(reinterpret_cast<char*>(&facet.attribute), 2);

        object.facets.push_back(facet);
    }
    input_stl.close();
}

void ascii_read(const fs::path& file, STL& object) {
    std::ifstream input_stl(file);
    std::string readed = "";
    std::getline(input_stl, readed);
    readed = std::string(readed.begin() + 6, readed.end());

    for (size_t i = 0; i < 80 && i < readed.size(); ++i) {
        object.header[i] = readed[i];
    }

    while(!input_stl.eof()) {
        Facet facet;
        bool flag = true;
        int vertex_count = 0;
        for (int i = 0; i < 7; ++i) {
            std::getline(input_stl, readed);
            if (readed.find("endsolid") != std::string::npos || readed == "") {
                flag = false;
                break;
            }

            if (readed.find("outer loop") != std::string::npos ||
                readed.find("endloop") != std::string::npos ||
                readed.find("endfacet") != std::string::npos) {
                continue;
            }

            if (readed.find("facet normal") != std::string::npos) {
                int index_counter = 0;
                for (auto it = readed.begin() + 13, end = readed.end();
                        it < end; ++it) {
                    std::string fut_number = "";
                    while(*it != ' ' && it != end) {
                        fut_number += *it;
                        ++it;
                    }
                    if (fut_number != "") {
                        facet.normal[index_counter] = std::stof(fut_number);
                        ++index_counter;
                    }
                }
            }


            if (readed.find("vertex") != std::string::npos) {
                ++vertex_count;
                int index_counter = 0;
                for (auto it = readed.begin() + 7, end = readed.end();
                        it < end; ++it) {
                    std::string fut_number = "";
                    while (*it != ' ' && it != end) {
                        fut_number += *it;
                        ++it;
                    }
                    if (fut_number != "") {
                        auto point = std::stof(fut_number);
                        if (vertex_count == 1) {
                            facet.first_vertex[index_counter] = point;
                        } else if (vertex_count == 2) {
                            facet.second_vertex[index_counter] = point;
                        } else if (vertex_count == 3) {
                            facet.third_vertex[index_counter] = point;
                        }
                        ++index_counter;
                    }
                }
            }
        }
        if (flag)
            object.facets.push_back(facet);
    }

    input_stl.close();
}

std::tuple<STL, fs::path, bool> read(const Parser& parser) {
    STL object;
    fs::path file;
    bool is_binary_file;
    bool to_edit = (parser.is_rotate || parser.is_move || parser.is_scale);
    bool to_convert = (parser.is_to_ascii || parser.is_to_binary);

    if (!parser.is_diff_location) {
        file = find_file();
    } else {
        file = parser.specify_file;
    }

    if (!to_convert && !to_edit && !parser.is_toposet) {
        check_location_and_copy(file);
        std::exit(0);
    }

    if (is_binary(file)) {
        if (parser.is_to_binary && !to_edit && !parser.is_toposet) {
            check_location_and_copy(file);
            std::exit(0);
        }
        binary_read(file, object);
        is_binary_file = true;
    } else {
        if (parser.is_to_ascii && !to_edit && !parser.is_toposet) {
            check_location_and_copy(file);
            std::exit(0);
        }
        ascii_read(file, object);
        is_binary_file = false;
    }

    std::tuple<STL, fs::path, bool> result{object, file, is_binary_file};

    return result;
}
