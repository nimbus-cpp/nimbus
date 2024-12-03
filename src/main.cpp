#define TOML11_PRESERVE_ORDER 1
#include <CLI/CLI.hpp>
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <toml.hpp>

void createToml(const std::string &projectName, const std::string &path) {
  // [project]
  toml::table project{{"name", projectName},
                      {"version", "0.1.0"},
                      {"authors", toml::array{"Your Name <you@example.com>"}}};

  // [build]
  toml::table build{
      {"compiler", "g++"}, {"standard", "c++20"}, {"build_type", "Debug"}};

  // Create the main TOML table with sub-tables in the desired order
  toml::table config{{"project", project}, {"build", build}};

  // Write the TOML value to the file
  std::ofstream file(path);
  if (file.is_open()) {
    file << toml::value(config); // Cast to toml::value for output
    file.close();
  } else {
    return;
  }
}

int main(int argc, char **argv) {
  CLI::App app{"nimbus - C++ Build System and Package Manager"};

  // Create a subcommand 'init' with alias 'i'
  auto init =
      app.add_subcommand("init", "Initialize a new project")->alias("i");

  // Optional argument 'project_name' for the 'init' subcommand
  std::string projectName;
  init->add_option("project_name", projectName, "Name of the project")
      ->default_val("");

  // Parse the command-line arguments
  CLI11_PARSE(app, argc, argv);

  // Check if 'init' subcommand was invoked
  if (*init) {
    std::cout << "Command: init" << std::endl;
    std::cout << "Project Name: " << projectName << std::endl;

    if (!projectName.empty()) {
      std::filesystem::create_directory(projectName);
      std::filesystem::create_directory(std::format("{}/src", projectName));
      std::filesystem::create_directory(std::format("{}/include", projectName));
      createToml(projectName, std::format("{}/nimbus.toml", projectName));
    } else {
      std::filesystem::create_directory("src");
      std::filesystem::create_directory("include");
      createToml(std::filesystem::current_path().filename().string(),
                 "nimbus.toml");
    }

  } else {
    // If no subcommand is provided, display help
    std::cout << app.help() << std::endl;
  }

  return 0;
}
