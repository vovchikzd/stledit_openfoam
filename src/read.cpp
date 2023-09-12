#include "read.h"

void binary_read(const fs::path& file, STL& object) {
    std::ifstream input_stl(file);
    uint32_t facets_number;

    char buffer[80]{};
    input_stl.read(buffer, 80);
    input_stl.read(reinterpret_cast<char*>(&facets_number), 4);

    std::string new_name = file.stem().string();
    for (size_t i = 0; i < 80; ++i) {
        if (i < new_name.size()) {
            object.header[i] = new_name[i];
        } else {
            object.header[i] = '\0';
        }
    }

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

    std::string new_name = file.stem().string();
    for (size_t i = 0; i < 80; ++i) {
        if (i < new_name.size()) {
            object.header[i] = new_name[i];
        } else {
            object.header[i] = '\0';
        }
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

std::vector<STL> read(const std::vector<fs::path>& files) {
    std::vector<STL> result;

    for (auto file: files) {
        STL object;
        if(is_binary(file)) {
            binary_read(file, object);
        } else {
            ascii_read(file, object);
        }

        result.push_back(object);
    }

    return result;
}
