/// Comment

module;

#include <cstdlib>
#include <filesystem>
#include <format>
#include <iostream>
#include <optional>
#include <toml.hpp>

export module build;

/// Comment
namespace build::constants {
constexpr std::string_view BUILD_DIR = "build";
constexpr std::string_view COMPILER = "compiler";
constexpr std::string_view STANDARD = "standard";
constexpr std::string_view NAME = "name";
constexpr std::string_view CONFIG_FILE = "nimbus.toml";

constexpr std::string_view SECTION_BUILD = "build";
constexpr std::string_view SECTION_PROJECT = "project";
} // namespace build::constants

namespace build {

/// Comment
void createBuildDirectory() {
  if (std::filesystem::is_directory(constants::BUILD_DIR)) {
    return;
  }

  std::filesystem::create_directory(constants::BUILD_DIR);
}

/// Comment
std::optional<std::string> extractConfigValue(std::string_view section,
                                              std::string_view key) {
  try {
    auto config = toml::parse(std::string(constants::CONFIG_FILE));

    if (config.contains(std::string(section))) {
      const auto &sectionTable = toml::find(config, std::string(section));
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

/// Comment
export void process() {
  createBuildDirectory();

  const auto compiler =
      extractConfigValue(constants::SECTION_BUILD, constants::COMPILER);
  if (!compiler.has_value()) {
    std::cerr << "Error! " << constants::COMPILER
              << " not provided in nimbus.toml" << std::endl;
    return;
  }

  const auto standard =
      extractConfigValue(constants::SECTION_BUILD, constants::STANDARD);
  if (!standard.has_value()) {
    std::cerr << "Error! " << constants::STANDARD
              << " not provided in nimbus.toml" << std::endl;
    return;
  }

  const auto projectName =
      extractConfigValue(constants::SECTION_PROJECT, constants::NAME);
  if (!projectName.has_value()) {
    std::cerr << "Error! " << constants::NAME << " not provided in nimbus.toml"
              << std::endl;
    return;
  }

  const std::string command =
      std::format("{} -std={} src/*.cpp -o {}/{}", compiler.value(),
                  standard.value(), constants::BUILD_DIR, projectName.value());

  if (auto result = std::system(command.c_str())) {
    std::cerr << "Error! Build failed with error code: " << result << std::endl;
    return;
  }
}

} // namespace build
