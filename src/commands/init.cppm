/**
 * @file init.cpp
 * @brief This module handles the initialization of a new C++ project.
 *
 * The `init` module provides functionality for setting up the basic structure
 * of a C++ project, including creating directories and a configuration file.
 * It supports both named project directories and initialization of the current
 * directory as a project.
 *
 * Features:
 * - Create a project directory with a given name.
 * - Set up internal `src` and `include` directories.
 * - Generate a `nimbus.toml` configuration file.
 */

module;

#include <filesystem>
#include <format>
#include <fstream>
#include <string_view>

export module init;

namespace init {

/**
 * @brief Creates a project directory with the specified name.
 *
 * @param projectName The name of the project directory to be created.
 */
void createProjectDirectory(std::string_view projectName) {
  std::filesystem::create_directory(projectName);
}

/**
 * @brief Creates internal directories (src and include) within a given path.
 *
 * @param path The base path where the directories will be created.
 */
void createInternalDirectories(std::string_view path) {
  std::filesystem::create_directory(std::format("{}/src", path));
  std::filesystem::create_directory(std::format("{}/include", path));
}

/**
 * @brief Creates a configuration file (`nimbus.toml`) for the project.
 *
 * @param projectName The name of the project, included in the configuration.
 * @param path The path where the configuration file will be created.
 */
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

/**
 * @brief Handles the initialization of a new project.
 *
 * If a project name is provided, creates a new project directory and its
 * internal structure along with a configuration file. If no name is provided,
 * initializes the current directory as a project.
 *
 * @param projectName The name of the project. If empty, the current directory
 * will be used.
 */
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
