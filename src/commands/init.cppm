/// Comment
module;

#include <format>
#include <fstream>
#include <string_view>

export module init;

namespace init {

/// Comment
void createProjectDirectory(std::string_view projectName) {
  std::filesystem::create_directory(projectName);
}

/// Comment
void createInternalDirectories(std::string_view path) {
  std::filesystem::create_directory(std::format("{}/src", path));
  std::filesystem::create_directory(std::format("{}/include", path));
}

/// Comment
void createConfigurationFile(std::string_view projectName,
                             std::string_view path) {
  std::string tomlContent =
      std::format("[project]\n"
                  "name = \"{}\"\n"
                  "version = \"0.1.0\"\n"
                  "authors = [\"Your Name <you@example.com>\"]\n\n"
                  "[build]\n"
                  "compiler = \"clang++\"\n"
                  "standard = \"c++20\"\n"
                  "build_type = \"Debug\"\n",
                  projectName);

  std::ofstream file(std::format("{}/nimbus.toml", path));
  if (file.is_open()) {
    file << tomlContent;
    file.close();
  }
}

/// Comment
export void handle(std::string_view projectName) {
  if (projectName.empty()) {
    std::string_view CURRENT_DIRECTORY = ".";
    createInternalDirectories(CURRENT_DIRECTORY);
    createConfigurationFile(std::filesystem::current_path().filename().string(),
                            CURRENT_DIRECTORY);
  } else {
    createProjectDirectory(projectName);
    createInternalDirectories(projectName);
    createConfigurationFile(projectName, projectName);
  }
}
} // namespace init
