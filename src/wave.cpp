#include "wave.hpp"

#include <ios>

void create_wave_stl(const std::vector<STL>& objects) {
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

  auto x_min_int = static_cast<int>(x_min - 1);
  auto x_max_int = static_cast<int>(x_max + 1);

  auto y_min_int = static_cast<int>(y_min - 1);
  auto y_max_int = static_cast<int>(y_max + 1);

  auto z_min_int = static_cast<int>(z_min - 1);
  auto z_max_int = static_cast<int>(z_max + 1);

  std::vector<std::pair<int, int>> extremes{
      {x_min_int, x_max_int}, {y_min_int, y_max_int}, {z_min_int, z_max_int}};

  write_wave_file(extremes);
}

void write_wave_file(const std::vector<std::pair<int, int>>& extremes) {
  fs::path constant("constant");
  fs::path triSurface(constant / "triSurface");

  if (!fs::is_directory(constant) || !fs::exists(constant)) {
    fs::create_directory(constant);
  }
  if (!fs::is_directory(triSurface) || !fs::exists(triSurface)) {
    fs::create_directory(triSurface);
  }

  auto [x_min, x_max] = extremes[0];
  auto [y_min, y_max] = extremes[1];
  auto [z_min, z_max] = extremes[2];

  auto length = x_max - x_min;
  auto width = y_max - y_min;
  auto height = (1.5 * (z_max - z_min));

  int y_left_back = (y_max - width / 2) + length;
  int y_right_back = (y_min + width / 2) - length;

  int z_up = z_max + (height / 2) + 1;
  int z_bottom = z_min - ((height / 2) - 1);

  int x_forward = (x_max + 0.2 * length) + 1;
  int x_back = (x_min - (length * 6) / 7) - 1;

  std::ofstream output_file(triSurface / "wave.stl");
  output_file << "solid wave\n";

  output_file << "facet normal " << std::fixed << 0.0 << ' ' << 0.0 << ' '
              << 10.0 << '\n';
  output_file << "outer loop\n";
  output_file << "vertex " << std::fixed << x_back * 1.0 << ' '
              << y_right_back * 1.0 << ' ' << z_up * 1.0 << '\n'
              << "vertex " << x_forward * 1.0 << ' ' << y_min * 1.0 << ' '
              << z_up * 1.0 << '\n'
              << "vertex " << x_forward * 1.0 << ' ' << y_max * 1.0 << ' '
              << z_up * 1.0 << '\n'
              << "endloop\nendfacet\n";

  output_file << "facet normal " << std::fixed << 0.0 << ' ' << 0.0 << ' '
              << 10.0 << '\n';
  output_file << "outer loop\n";
  output_file << "vertex " << std::fixed << x_back * 1.0 << ' '
              << y_right_back * 1.0 << ' ' << z_up * 1.0 << '\n'
              << "vertex " << x_back * 1.0 << ' ' << y_left_back * 1.0 << ' '
              << z_up * 1.0 << '\n'
              << "vertex " << x_forward * 1.0 << ' ' << y_max * 1.0 << ' '
              << z_up * 1.0 << '\n'
              << "endloop\nendfacet\n";

  output_file << "facet normal " << std::fixed << 0.0 << ' ' << 0.0 << ' '
              << -10.0 << '\n';
  output_file << "outer loop\n";
  output_file << "vertex " << std::fixed << x_back * 1.0 << ' '
              << y_right_back * 1.0 << ' ' << z_bottom * 1.0 << '\n'
              << "vertex " << x_forward * 1.0 << ' ' << y_min * 1.0 << ' '
              << z_bottom * 1.0 << '\n'
              << "vertex " << x_forward * 1.0 << ' ' << y_max * 1.0 << ' '
              << z_bottom * 1.0 << '\n'
              << "endloop\nendfacet\n";

  output_file << "facet normal " << std::fixed << 0.0 << ' ' << 0.0 << ' '
              << -10.0 << '\n';
  output_file << "outer loop\n";
  output_file << "vertex " << std::fixed << x_back * 1.0 << ' '
              << y_right_back * 1.0 << ' ' << z_bottom * 1.0 << '\n'
              << "vertex " << x_back * 1.0 << ' ' << y_left_back * 1.0 << ' '
              << z_bottom * 1.0 << '\n'
              << "vertex " << x_forward * 1.0 << ' ' << y_max * 1.0 << ' '
              << z_bottom * 1.0 << '\n'
              << "endloop\nendfacet\n";

  output_file << "facet normal " << std::fixed << -10.0 << ' ' << 0.0 << ' '
              << 0.0 << '\n';
  output_file << "outer loop\n";
  output_file << "vertex " << std::fixed << x_back * 1.0 << ' '
              << y_right_back * 1.0 << ' ' << z_bottom * 1.0 << '\n'
              << "vertex " << x_back * 1.0 << ' ' << y_right_back * 1.0 << ' '
              << z_up * 1.0 << '\n'
              << "vertex " << x_back * 1.0 << ' ' << y_left_back * 1.0 << ' '
              << z_up * 1.0 << '\n'
              << "endloop\nendfacet\n";

  output_file << "facet normal " << std::fixed << -10.0 << ' ' << 0.0 << ' '
              << 0.0 << '\n';
  output_file << "outer loop\n";
  output_file << "vertex " << std::fixed << x_back * 1.0 << ' '
              << y_right_back * 1.0 << ' ' << z_bottom * 1.0 << '\n'
              << "vertex " << x_back * 1.0 << ' ' << y_left_back * 1.0 << ' '
              << z_bottom * 1.0 << '\n'
              << "vertex " << x_back * 1.0 << ' ' << y_left_back * 1.0 << ' '
              << z_up * 1.0 << '\n'
              << "endloop\nendfacet\n";

  output_file << "facet normal " << std::fixed << 10.0 << ' ' << 0.0 << ' '
              << 0.0 << '\n';
  output_file << "outer loop\n";
  output_file << "vertex " << std::fixed << x_forward * 1.0 << ' '
              << y_min * 1.0 << ' ' << z_bottom * 1.0 << '\n'
              << "vertex " << x_forward * 1.0 << ' ' << y_min * 1.0 << ' '
              << z_up * 1.0 << '\n'
              << "vertex " << x_forward * 1.0 << ' ' << y_max * 1.0 << ' '
              << z_up * 1.0 << '\n'
              << "endloop\nendfacet\n";

  output_file << "facet normal " << std::fixed << 10.0 << ' ' << 0.0 << ' '
              << 0.0 << '\n';
  output_file << "outer loop\n";
  output_file << "vertex " << std::fixed << x_forward * 1.0 << ' '
              << y_min * 1.0 << ' ' << z_bottom * 1.0 << '\n'
              << "vertex " << x_forward * 1.0 << ' ' << y_max * 1.0 << ' '
              << z_bottom * 1.0 << '\n'
              << "vertex " << x_forward * 1.0 << ' ' << y_max * 1.0 << ' '
              << z_up * 1.0 << '\n'
              << "endloop\nendfacet\n";

  output_file << "facet normal " << std::fixed
              << (z_up - z_bottom) * (y_min - y_right_back) * 1.0 << ' '
              << (x_forward - x_back) * (z_bottom - z_up) * 1.0 << ' ' << 0.0
              << '\n';
  output_file << "outer loop\n";
  output_file << "vertex " << std::fixed << x_back * 1.0 << ' '
              << y_right_back * 1.0 << ' ' << z_up * 1.0 << '\n'
              << "vertex " << x_forward * 1.0 << ' ' << y_min * 1.0 << ' '
              << z_up * 1.0 << '\n'
              << "vertex " << x_forward * 1.0 << ' ' << y_min * 1.0 << ' '
              << z_bottom * 1.0 << '\n'
              << "endloop\nendfacet\n";

  output_file << "facet normal " << std::fixed
              << (z_up - z_bottom) * (y_min - y_right_back) * 1.0 << ' '
              << (x_forward - x_back) * (z_bottom - z_up) * 1.0 << ' ' << 0.0
              << '\n';
  output_file << "outer loop\n";
  output_file << "vertex " << std::fixed << x_back * 1.0 << ' '
              << y_right_back * 1.0 << ' ' << z_up * 1.0 << '\n'
              << "vertex " << x_forward * 1.0 << ' ' << y_min * 1.0 << ' '
              << z_bottom * 1.0 << '\n'
              << "vertex " << x_back * 1.0 << ' ' << y_right_back * 1.0 << ' '
              << z_bottom * 1.0 << '\n'
              << "endloop\nendfacet\n";

  output_file << "facet normal " << std::fixed
              << (z_up - z_bottom) * (y_max - y_left_back) * 1.0 << ' '
              << (x_forward - x_back) * (z_bottom - z_up) * 1.0 << ' ' << 0.0
              << '\n';
  output_file << "outer loop\n";
  output_file << "vertex " << std::fixed << x_back * 1.0 << ' '
              << y_left_back * 1.0 << ' ' << z_up * 1.0 << '\n'
              << "vertex " << x_forward * 1.0 << ' ' << y_max * 1.0 << ' '
              << z_up * 1.0 << '\n'
              << "vertex " << x_forward * 1.0 << ' ' << y_max * 1.0 << ' '
              << z_bottom * 1.0 << '\n'
              << "endloop\nendfacet\n";

  output_file << "facet normal " << std::fixed
              << (z_up - z_bottom) * (y_max - y_left_back) * 1.0 << ' '
              << (x_forward - x_back) * (z_bottom - z_up) * 1.0 << ' ' << 0.0
              << '\n';
  output_file << "outer loop\n";
  output_file << "vertex " << std::fixed << x_back * 1.0 << ' '
              << y_left_back * 1.0 << ' ' << z_up * 1.0 << '\n'
              << "vertex " << x_forward * 1.0 << ' ' << y_max * 1.0 << ' '
              << z_bottom * 1.0 << '\n'
              << "vertex " << x_back * 1.0 << ' ' << y_left_back * 1.0 << ' '
              << z_bottom * 1.0 << '\n'
              << "endloop\nendfacet\n";

  output_file << "endsolid wave\n";
  output_file.close();
}
