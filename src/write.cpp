#include "write.hpp"

void write_ascii(const std::vector<STL>& objects, const fs::path& file) {
  std::ofstream output_file(file);
  for (auto object : objects) {
    std::string name(object.header);
    output_file << "solid " << name << '\n';
    for (auto facet : object.facets) {
      output_file << "facet normal " << std::fixed << facet.normal[0] << ' '
                  << facet.normal[1] << ' ' << facet.normal[2] << '\n';

      output_file << "outer loop\n";

      output_file << "vertex " << std::fixed << facet.first_vertex[0] << ' '
                  << facet.first_vertex[1] << ' ' << facet.first_vertex[2]
                  << '\n';

      output_file << "vertex " << std::fixed << facet.second_vertex[0] << ' '
                  << facet.second_vertex[1] << ' ' << facet.second_vertex[2]
                  << '\n';

      output_file << "vertex " << std::fixed << facet.third_vertex[0] << ' '
                  << facet.third_vertex[1] << ' ' << facet.third_vertex[2]
                  << '\n';

      output_file << "endloop\nendfacet\n";
    }

    output_file << "endsolid " << name << '\n';
  }
  output_file.close();
}

void write_binary(const std::vector<STL>& objects, const fs::path& file) {
  uint32_t facets_number = 0;
  for (auto object : objects) {
    facets_number += static_cast<uint32_t>(object.facets.size());
  }

  char header[80]{};
  std::string name = file.stem().string();
  for (size_t i = 0; i < 80 && i < name.size(); ++i) {
    header[i] = name[i];
  }

  std::ofstream output_file(file);

  output_file.write(header, 80);
  output_file.write(reinterpret_cast<char*>(&facets_number), 4);

  for (auto object : objects) {
    for (auto facet : object.facets) {
      output_file.write(reinterpret_cast<char*>(&facet.normal), 12);
      output_file.write(reinterpret_cast<char*>(&facet.first_vertex), 12);
      output_file.write(reinterpret_cast<char*>(&facet.second_vertex), 12);
      output_file.write(reinterpret_cast<char*>(&facet.third_vertex), 12);
      output_file.write(reinterpret_cast<char*>(&facet.attribute), 2);
    }
  }

  output_file.close();
}

void write(const std::vector<STL>& objects, const fs::path& file, bool binary) {
  if (binary)
    write_binary(objects, file);
  else
    write_ascii(objects, file);
}
