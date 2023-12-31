#include "toposet.hpp"

#include "tools.hpp"

void write_toposet(const std::vector<std::pair<float, float>>& extremes,
                   const size_t& file_numbers) {
  fs::path system("system");
  fs::path refinemesh(system / "refineMeshDict");
  std::string common_name = "topoSetDict.";

  auto [x_min, x_max] = extremes[0];
  auto [y_min, y_max] = extremes[1];
  auto [z_min, z_max] = extremes[2];

  if (!fs::is_directory(system) || !fs::exists(system)) {
    fs::create_directory(system);
  }

  auto length = std::abs(x_max - x_min);
  auto x_forward = 0.1 * length * std::pow(2, file_numbers - 1);
  auto x_back = 0.2 * length * std::pow(2, file_numbers - 1);
  auto z_up = 0.025 * length * std::pow(2, file_numbers - 1);
  auto z_down = 0.05 * length * std::pow(2, file_numbers - 1);
  auto y_side = z_down;

  for (size_t i = 1; i <= file_numbers; ++i) {
    std::string file_name = common_name + to_string(i);
    std::ofstream output_file(system / file_name);

    output_file << "FoamFile {\n"
                << "\tversion\t2.0;\n"
                << "\tformat\tascii;\n"
                << "\tclass\tdictionary;\n"
                << "\tlocation\t\"system\";\n"
                << "\tobject\ttopoSetDict;\n}\n\n";

    output_file << "actions ({\n"
                << "\tname\tc0;\n"
                << "\ttype\tcellSet;\n"
                << "\taction\tnew;\n"
                << "\tsource\tboxToCell;\n"
                << "\tsourseInfo {\n"
                << "\t\tbox (" << x_min - x_back << ' ' << y_min - y_side << ' '
                << z_min - z_down << ") (" << x_max + x_forward << ' '
                << y_max + y_side << ' ' << z_max + z_up << ");\n\t}\n});";

    output_file.close();
    x_forward /= 2;
    x_back /= 2;
    z_up /= 2;
    z_down /= 2;
    y_side /= 2;
  }

  std::ofstream output_refine(refinemesh);

  output_refine << "FoamFile {\n"
                << "\tversion\t2.0;\n"
                << "\tformat\tascii;\n"
                << "\tclass\tdictionary;\n"
                << "\tlocation\t\"system\";\n"
                << "\tobject\trefineMeshDict;\n}\n\n";

  output_refine << "set\tc0;\n\n";

  output_refine << "coordinateSystem\tglobal;\n\n";

  output_refine << "globalCoeffs {\n"
                << "\ttan1\t(1 0 0);\n"
                << "\ttan2\t(0 1 0);\n"
                << "\t// tan3\t(0 0 1);\n}\n\n";

  output_refine << "patchLocalCoeffs {\n"
                << "\tpatch\toutside;\n"
                << "\ttan1\t(1 0 0);\n}\n\n";

  output_refine << "useHexTopology\tno;\n"
                << "geometricCut\tyes;\n"
                << "writeMesh\tno;\n";

  output_refine.close();
}

void create_toposet(const std::vector<STL>& objects,
                    const size_t& file_number) {
  auto first_facets = objects[0].facets;

  auto first_extremes = facet_minmax(first_facets[0]);

  auto [x_min, x_max] = first_extremes[0];
  auto [y_min, y_max] = first_extremes[1];
  auto [z_min, z_max] = first_extremes[2];

  for (auto object : objects) {
    for (auto facet : object.facets) {
      auto extremes = facet_minmax(facet);

      auto [x_local_min, x_local_max] = extremes[0];
      auto [y_local_min, y_local_max] = extremes[1];
      auto [z_local_min, z_local_max] = extremes[2];

      x_min = x_local_min < x_min ? x_local_min : x_min;
      x_max = x_local_max > x_max ? x_local_max : x_max;

      y_min = y_local_min < y_min ? y_local_min : y_min;
      y_max = y_local_max > y_max ? y_local_max : y_max;

      z_min = z_local_min < z_min ? z_local_min : z_min;
      z_max = z_local_max > z_max ? z_local_max : z_max;
    }
  }

  std::vector<std::pair<float, float>> final_extremes{
      {x_min, x_max}, {y_min, y_max}, {z_min, z_max}};

  write_toposet(final_extremes, file_number);
}
