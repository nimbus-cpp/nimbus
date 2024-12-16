/**
 * @file build.cppm
 * @brief This module handles the construction and execution of the build
 * process.
 *
 * The `build` module is responsible for orchestrating the project's build
 * steps, including creating necessary directories, reading configuration values
 * from the project's `nimbus.toml` file, and executing the compile command.
 *
 * Features:
 * - Create a build directory if it does not exist.
 * - Extract configuration values for compiler, standard, build type, etc.
 * - Construct a build command string and execute it.
 */

module;

// std
#include <cstdlib>
#include <filesystem>
#include <format>
#include <iostream>
#include <optional>

// external
#include <toml.hpp>

// internal
#include "../../include/constants/config_toml.hpp"
#include "../../include/constants/directory.hpp"

export module build;

namespace build {
/**
 * @brief Creates the build directory if it does not already exist.
 *
 * Ensures that a dedicated build directory is available for compiled
 * objects and the final executable.
 */
void createBuildDirectory() {
  if (std::filesystem::is_directory(toString(constants::Directory::Build))) {
    return;
  }

  std::filesystem::create_directory(toString(constants::Directory::Build));
}

/**
 * @brief Extracts a configuration value from the project's `nimbus.toml` file.
 *
 * Attempts to locate and return the value for the given `table` and `key`
 * in the configuration file. If the value cannot be found or the file fails
 * to parse, it returns `std::nullopt`.
 *
 * @param table The configuration table name.
 * @param key The key within the given table.
 * @return An optional string with the configuration value if found;
 *         `std::nullopt` otherwise.
 */
std::optional<std::string> extractConfigValue(std::string_view table,
                                              std::string_view key) {
  try {
    auto config = toml::parse(std::string(constants::ConfigFile));

    if (config.contains(std::string(table))) {
      const auto &sectionTable = toml::find(config, std::string(table));
      if (sectionTable.contains(std::string(key))) {
        auto &value =
            toml::find_or<std::string>(sectionTable, std::string(key), "");
        if (!value.empty())
          return value;
      }
    }
  } catch (const toml::exception &e) {
    std::cerr << "Error! Failed reading nimbus.toml file: " << e.what()
              << std::endl;
  }

  return std::nullopt;
}

/**
 * @brief Logs a standardized error message for a missing configuration
 * parameter.
 *
 * This function is used to inform the user that a required configuration
 * parameter (identified by its table and key) is not provided in the
 * `nimbus.toml` file. It prints a clear error message to `stderr`.
 *
 * @param tbl The configuration table name.
 * @param key The configuration key that is missing.
 */
void logConfigErrorMessage(constants::Table tbl, constants::Key key) noexcept {
  std::cerr << "Error! " << toString(tbl) << "." << toString(key)
            << " not provided in " << constants::ConfigFile << std::endl;
}

/**
 * @brief Translates a known build type string into corresponding compiler
 * flags.
 *
 * Given a human-readable build type string (e.g., "Debug", "Release",
 * "RelWithDebInfo", "MinSizeRel"), this function modifies the input string in
 * place to contain the appropriate compiler flags.
 *
 * @param buildType A reference to the build type string to translate. If
 * recognized, it is replaced with the corresponding compiler flags.
 * @return `true` if the build type was recognized and translated, `false`
 * otherwise.
 */
bool translateBuildType(std::string &buildType) noexcept {
  if (buildType == "Debug") {
    buildType = "-g -O0 -DDEBUG";
    return true;
  } else if (buildType == "Release") {
    buildType = "-O2 -DRELEASE";
    return true;
  } else if (buildType == "RelWithDebInfo") {
    buildType = "-g -O2 -DRELWITHDEBINFO";
    return true;
  } else if (buildType == "MinSizeRel") {
    buildType = "-Os -DMINSIZEREL";
    return true;
  } else {
    return false;
  }
}

/**
 * @brief Constructs the build command string used to compile the project's
 * source files.
 *
 * Reads required parameters such as the compiler, standard, build type, and
 * project name from the `nimbus.toml` file. Then, constructs a complete shell
 * command string that will invoke the compiler with all necessary flags and
 * include all source files.
 *
 * @return An optional string containing the build command if all required
 * configurations are present and valid, `std::nullopt` otherwise.
 */
std::optional<std::string> constructBuildCommand() noexcept {
  const auto compiler = extractConfigValue(toString(constants::Table::Build),
                                           toString(constants::Key::Compiler));
  if (!compiler.has_value()) {
    logConfigErrorMessage(constants::Table::Build, constants::Key::Compiler);
    return std::nullopt;
  }

  const auto standard = extractConfigValue(toString(constants::Table::Build),
                                           toString(constants::Key::Standard));
  if (!standard.has_value()) {
    logConfigErrorMessage(constants::Table::Build, constants::Key::Standard);
    return std::nullopt;
  }

  auto buildType = extractConfigValue(toString(constants::Table::Build),
                                      toString(constants::Key::BuildType));
  if (!buildType.has_value() || !translateBuildType(buildType.value())) {
    logConfigErrorMessage(constants::Table::Build, constants::Key::BuildType);
    return std::nullopt;
  }

  const auto projectName = extractConfigValue(
      toString(constants::Table::Project), toString(constants::Key::Name));
  if (!projectName.has_value()) {
    logConfigErrorMessage(constants::Table::Project, constants::Key::Name);
    return std::nullopt;
  }

  constexpr std::string_view findSubcommand =
      "$(find {} -type f \\( -name \"*.cpp\" -o "
      "-name \"*.c\" -o -name \"*.cxx\" \\))";

  return std::format(
      "{} -std={} {} {} -o {}/{}", compiler.value(), standard.value(),
      std::format(findSubcommand, toString(constants::Directory::Src)),
      buildType.value(), toString(constants::Directory::Build),
      projectName.value());
}

/**
 * @brief Executes the provided shell command and reports build success or
 * failure.
 *
 * Uses `std::system` to run the given build command. If the command fails,
 * prints an error message with the returned status code. Otherwise, indicates
 * that the build process has finished successfully.
 *
 * @param command The shell command to be executed.
 */
void executeCommand(const std::string &command) {
  if (const auto result = std::system(command.c_str())) {
    std::cerr << "Error! Build failed with error code: " << result << std::endl;
    return;
  }

  std::cout << "Build process finished!" << std::endl;
}

/**
 * @brief Orchestrates the build process of the current project.
 *
 * Ensures the build directory exists, constructs the build command from
 * configuration values, and executes it if all required values are present.
 */
export void process() {
  createBuildDirectory();

  auto command = constructBuildCommand();
  if (command.has_value())
    executeCommand(command.value());
}
} // namespace build
