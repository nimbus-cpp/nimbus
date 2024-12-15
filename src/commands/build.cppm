/// Comment

module;

#include <cstdlib>
#include <filesystem>
#include <format>
#include <iostream>
#include <optional>

export module build;

/// Comment
namespace build::constants {
constexpr std::string_view BUILD_DIR = "build";
constexpr std::string_view COMPILER = "compiler";
constexpr std::string_view STANDARD = "standard";
constexpr std::string_view PROJECT_NAME = "project_name";
} // namespace build::constants

namespace build {

/// Comment
void createBuildDirectory() {
  std::filesystem::create_directory(constants::BUILD_DIR);
}

/// Comment
std::optional<std::string> extractConfigValue(std::string_view key) {
  if (key == "compiler")
    return "g++";
  if (key == "project_name")
    return "newprj";
  if (key == "standard")
    return "c++20";

  return std::nullopt;
}

/// Comment
export void process() {
  // TODO: Check if exists
  createBuildDirectory();

  const auto compiler = extractConfigValue(constants::COMPILER);
  if (!compiler.has_value()) {
    std::cerr << "Error! \"compiler\" not provided in nimbus.toml" << std::endl;
    return;
  }

  const auto standard = extractConfigValue(constants::STANDARD);
  if (!standard.has_value()) {
    std::cerr << "Error! \"standard\" not provided in nimbus.toml" << std::endl;
    return;
  }

  const auto projectName = extractConfigValue(constants::PROJECT_NAME);
  if (!projectName.has_value()) {
    std::cerr << "Error! \"project_name\" not provided in nimbus.toml"
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
