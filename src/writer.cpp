#include "../headers/writer.h"

void check_location() {
    fs::path constant("constant");
    fs::path triSurface(constant/"triSurface");

    if (!fs::is_directory(constant) || !fs::exists(constant)) {
        fs::create_directory(constant);
    }
    if (!fs::is_directory(triSurface) || !fs::exists(triSurface)) {
        fs::create_directory(triSurface);
    }
}

fs::path check_file(const fs::path& file, const std::string& name) {
    fs::path default_location(fs::path("constant")/fs::path("triSurface"));
    
    if (file.parent_path() == default_location) {
        if (file.filename().string() == name) {
            fs::copy(file, default_location/(name + ".bac"));
            return file;
        } else {
            fs::path result(default_location/name);
            return result;
        }
    } else {
        fs::path result(default_location/name);
        fs::copy(file, result);
        return result;
    }
}

void write_ascii(const STL& object, std::ofstream& output_file,
                 const std::string& name) {
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

void write_binary(const STL& object, std::ofstream& output_file) {
    output_file.write(object.header, 80);
    uint32_t facets_number = static_cast<uint32_t>(object.facets.size());
    output_file.write(reinterpret_cast<char*>(&facets_number), 4);

    for (auto facet: object.facets) {
        output_file.write(reinterpret_cast<char*>(&facet.normal), 12);
        output_file.write(reinterpret_cast<char*>(&facet.first_vertex), 12);
        output_file.write(reinterpret_cast<char*>(&facet.second_vertex), 12);
        output_file.write(reinterpret_cast<char*>(&facet.third_vertex), 12);
        output_file.write(reinterpret_cast<char*>(&facet.attribute), 2);
    }
}

void write(const STL& object, const fs::path& file,
           bool is_binary_file, const Parser& parser) {

    fs::path file_to_write;
    std::string name = file.filename().string();
    if (parser.out_name.string() != "out.stl")
        name = parser.out_name.string();

    check_location();
    file_to_write = check_file(file, name);

    if (parser.is_to_ascii || parser.is_to_binary) {
        std::exit(0);
    }

    std::ofstream output_file(file_to_write);

    if (is_binary_file) 
        write_binary(object, output_file);
    else 
        write_ascii(object, output_file, file_to_write.stem().string());

    output_file.close();

}
