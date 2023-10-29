#include "tools.hpp"

std::vector<fs::path> find_file() {
  fs::path constant("constant");
  fs::path triSurface("triSurface");
  std::vector<fs::path> result;

  if (!fs::is_directory(constant) || !fs::exists(constant)) {
    std::cerr << "The default file location doesn't exist.\n"
              << "Specify file location.\n";
    throw std::exception();
  } else if (!fs::is_directory(constant / triSurface) ||
             !fs::exists(constant / triSurface)) {
    std::cerr << "The default file location doesn't exist.\n"
              << "Specify file location.\n";
    throw std::exception();
  }

  if (fs::is_empty(constant / triSurface)) {
    std::cerr << "The default file location is empty.\n"
              << "Specify file location.\n";
    throw std::exception();
  }

  fs::directory_iterator default_location(constant / triSurface);
  std::vector<fs::directory_entry> files;

  for (auto file : default_location) {
    if (file.path().extension() == ".stl") {
      files.push_back(file);
    }
  }

  if (files.empty()) {
    std::cerr << "There isn't any .stl file in the default location.\n"
              << "Specify file location\n";
    throw std::exception();
  } else if (files.size() == 1) {
    result.push_back(files[0].path());
  } else {
    std::cout << "There are some *.stl files.\n"
              << "Choose option or cancel and specify location\n\n"
              << "Files:\n";

    size_t i = 0;
    for (; i < files.size(); ++i) {
      std::cout << i + 1 << ". " << files[i].path().filename().string() << '\n';
    }
    size_t all_files = i + 1;
    std::cout << all_files << ". Consider all of them\n";
    std::cout << "\nType a number (0 or Ctrl-c to cancel): ";
    size_t choice;
    std::cin >> choice;

    if (!choice) {
      std::exit(0);
    } else if (choice == all_files) {
      for (auto file : files) {
        result.push_back(file.path());
      }
    } else {
      result.push_back(files[choice - 1].path());
    }
  }

  return result;
}

void check_location_and_copy(const fs::path& file, const fs::path& new_name) {
  fs::path constant("constant");
  fs::path triSurface(constant / "triSurface");
  fs::path file_parent = file.parent_path();
  fs::path name;

  if (!new_name.empty()) {
    name = new_name;
  } else {
    name = file.filename();
  }

  if (!fs::is_directory(constant) || !fs::exists(constant)) {
    fs::create_directory(constant);
  }

  if (!fs::is_directory(triSurface) || !fs::exists(triSurface)) {
    fs::create_directory(triSurface);
  }

  if (!(file_parent == triSurface) || !(name == file.filename()))
    fs::copy(file, triSurface / name, fs::copy_options::overwrite_existing);
}

bool is_binary(const fs::path& file) {
  std::ifstream input_file(file);
  std::string first_word;
  std::getline(input_file, first_word);
  input_file >> first_word;
  input_file.close();

  if (first_word == "facet") {
    return false;
  } else {
    return true;
  }
}

fs::path get_write_file(const std::vector<fs::path>& files,
                        const fs::path& new_name) {
  fs::path result;
  fs::path constant("constant");
  fs::path triSurface(constant / "triSurface");

  if (!fs::is_directory(constant) || !fs::exists(constant)) {
    fs::create_directory(constant);
  }
  if (!fs::is_directory(triSurface) || !fs::exists(triSurface)) {
    fs::create_directory(triSurface);
  }

  if (!new_name.empty()) {
    result = triSurface / new_name;
  } else if (files.size() > 1) {
    result = triSurface / "out.stl";
  } else if (files.size() == 1) {
    std::string file_name = files[0].filename().string();
    if (fs::exists(triSurface / file_name)) {
      fs::copy(triSurface / file_name, triSurface / (file_name + ".bak"),
               fs::copy_options::overwrite_existing);
    }
    result = triSurface / file_name;
  }

  return result;
}

std::vector<std::pair<float, float>> facet_minmax(const Facet& facet) {
  std::vector<std::pair<float, float>> result;
  auto x_pair = std::minmax(
      {facet.first_vertex[0], facet.second_vertex[0], facet.third_vertex[0]});
  result.push_back(x_pair);

  auto y_pair = std::minmax(
      {facet.first_vertex[1], facet.second_vertex[1], facet.third_vertex[1]});
  result.push_back(y_pair);

  auto z_pair = std::minmax(
      {facet.first_vertex[2], facet.second_vertex[2], facet.third_vertex[2]});
  result.push_back(z_pair);

  return result;
}
