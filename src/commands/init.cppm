/**
 * @file init.cppm
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

/**
 * @namespace init::constants
 * @brief Contains constants used during project initialization.
 *
 * This namespace encapsulates all the constants required for initializing
 * a project, including directory names, configuration file names, and
 * default content templates. These constants are designed to promote
 * maintainability and consistency across the project initialization process.
 */
namespace init::constants {
constexpr std::string_view SRC_DIR = "src";
constexpr std::string_view INCLUDE_DIR = "include";
constexpr std::string_view CURRENT_DIR = ".";

constexpr std::string_view CONFIG_FILE = "nimbus.toml";
constexpr std::string_view CONFIG_FILE_CONTENT =
    "[project]\n"
    "name = \"{}\"\n"
    "version = \"0.1.0\"\n"
    "authors = [\"Your Name <you@example.com>\"]\n\n"
    "[build]\n"
    "compiler = \"clang++\"\n"
    "standard = \"c++20\"\n"
    "build_type = \"Debug\"\n";
} // namespace init::constants

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
  std::filesystem::create_directory(
      std::format("{}/{}", path, constants::SRC_DIR));
  std::filesystem::create_directory(
      std::format("{}/{}", path, constants::INCLUDE_DIR));
}

/**
 * @brief Creates a configuration file (`nimbus.toml`) for the project with
 * default values being set.
 *
 * @param projectName The name of the project, included in the configuration.
 * @param path The path where the configuration file will be created.
 */
void createConfigurationFile(std::string_view projectName,
                             std::string_view path) {
  std::ofstream file(std::format("{}/{}", path, constants::CONFIG_FILE));
  if (file.is_open()) {
    file << std::format(constants::CONFIG_FILE_CONTENT, projectName);
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
export void process(std::string_view projectName) {
  if (projectName.empty()) {
    createInternalDirectories(constants::CURRENT_DIR);
    createConfigurationFile(std::filesystem::current_path().filename().string(),
                            constants::CURRENT_DIR);
  } else {
    createProjectDirectory(projectName);
    createInternalDirectories(projectName);
    createConfigurationFile(projectName, projectName);
  }
}
} // namespace init
