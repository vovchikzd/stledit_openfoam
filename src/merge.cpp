#include "../headers/merge.h"
#include <ios>

void binary_merge(const fs::path& file, std::ofstream& output_file) {
    STL object;
    binary_read(file, object);

    std::string name = file.stem();
    output_file << "solid " << name << '\n';
    for (auto facet: object.facets) {
        output_file << "facet normal "<< std::fixed
                    << facet.normal[0] << ' '
                    << facet.normal[1] << ' '
                    << facet.normal[2] << '\n';

        output_file << "outer loop\n";

        output_file << "vertex " << std::fixed
                   << facet.first_vertex[0] << ' '
                   << facet.first_vertex[1] << ' '
                   << facet.first_vertex[2] << '\n';

        output_file << "vertex " << std::fixed
                    << facet.second_vertex[0] << ' '
                    << facet.second_vertex[1] << ' '
                    << facet.second_vertex[2] << '\n';

        output_file << "vertex " << std::fixed
                    << facet.third_vertex[0] << ' '
                    << facet.third_vertex[1] << ' '
                    << facet.third_vertex[2] << '\n';

        output_file << "endloop\nendfacet\n";
    }

    output_file << "endsolid " << name << '\n';
}

void ascii_merge(const fs::path& file, std::ofstream& output_file) {
    std::string name = file.stem();
    std::ifstream input_stl(file);

    output_file << "solid " << name << '\n';

    std::string readed;
    std::getline(input_stl, readed);
    while(!input_stl.eof()) {
        std::getline(input_stl, readed);
        if (readed == "") continue;
        else if (readed.find("endsolid") != std::string::npos) {
            output_file << "endsolid " << name << '\n';
            continue;
        } else {
            output_file << readed << '\n';
        }
    }
    input_stl.close();
}

void merge(const Parser& parser) {
    fs::path constant("constant");
    fs::path triSurface(constant/"triSurface");
    fs::path stl_file(triSurface/parser.out_name);

    if(!fs::is_directory(constant) || !fs::exists(constant)) {
        fs::create_directory(constant);
    }
    if (!fs::is_directory(triSurface) || !fs::exists(triSurface)) {
        fs::create_directory(triSurface);
    }

    std::ofstream output_file(stl_file);

    for (auto file: parser.merge_files) {
        
        if (is_binary(file)) {
            binary_merge(file, output_file);
        } else {
            ascii_merge(file, output_file);
        }
    }

    output_file.close();
    
}
