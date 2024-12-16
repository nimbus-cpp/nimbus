/// Comment

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

/// Comment
void createBuildDirectory() {
  if (std::filesystem::is_directory(toString(constants::Directory::Build))) {
    return;
  }

  std::filesystem::create_directory(toString(constants::Directory::Build));
}

/// Comment
std::optional<std::string> extractConfigValue(std::string_view section,
                                              std::string_view key) {
  try {
    auto config = toml::parse(std::string(constants::ConfigFile));

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

/// Comment example tbl.key
void logConfigErrorMessage(constants::Table tbl, constants::Key key) noexcept {
  std::cerr << "Error! " << toString(tbl) << "." << toString(key)
            << " not provided in " << constants::ConfigFile << std::endl;
}

/// Comment
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

/// Comment
std::optional<std::string> constructBuildCommand() {
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

/// Comment
void executeCommand(const std::string &command) {
  if (const auto result = std::system(command.c_str())) {
    std::cerr << "Error! Build failed with error code: " << result << std::endl;
    return;
  }

  std::cout << "Build process finished!" << std::endl;
}

/// Comment
export void process() {
  createBuildDirectory();

  auto command = constructBuildCommand();
  if (command.has_value())
    executeCommand(command.value());
}

} // namespace build
