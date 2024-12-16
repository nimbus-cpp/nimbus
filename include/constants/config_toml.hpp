/**
 * @file config_toml.hpp
 * @brief Defines constants and utility functions related to the project's
 * `nimbus.toml` configuration file.
 *
 * This header provides constants, enums, and helper functions for interacting
 * with the project's `nimbus.toml` file. It includes defaults for various
 * configuration values and mappings from enum types to their string
 * representations.
 */

#pragma once

#include <string_view>

namespace constants {
/**
 * @brief The name of the project's configuration file.
 */
constexpr std::string_view ConfigFile = "nimbus.toml";

// TODO: Automate using existing enum constants!
/**
 * @brief Provides default contents for the `nimbus.toml` configuration file.
 *
 * Returns a TOML-formatted string that outlines default project metadata and
 * build settings. This can be used when initializing a new project or when
 * generating the configuration file if one does not exist.
 *
 * @return A `std::string_view` containing the default configuration text.
 */
constexpr std::string_view getConfigFileDefaults() noexcept {
  return "[project]\n"
         "name = \"{}\"\n"
         "version = \"0.1.0\"\n"
         "authors = [\"Your Name <you@example.com>\"]\n\n"
         "[build]\n"
         "compiler = \"clang++\"\n"
         "standard = \"c++20\"\n"
         "build_type = \"Debug\"\n";
}

/**
 * @brief Enumerates the configuration sections (tables) within `nimbus.toml`.
 */
enum class Table { Project, Build };

/**
 * @brief Converts a `Table` enumeration value to its corresponding string
 * representation.
 *
 * @param tbl The `Table` enum value.
 * @return A `std::string_view` that corresponds to the given table (e.g.,
 * "project" or "build").
 */
constexpr std::string_view toString(Table tbl) noexcept {
  switch (tbl) {
  case Table::Project:
    return "project";
  case Table::Build:
    return "build";
  }
}

/**
 * @brief Enumerates the keys used within each configuration table of
 * `nimbus.toml`.
 */
enum class Key { Name, Version, Authors, Compiler, Standard, BuildType };

/**
 * @brief Converts a `Key` enumeration value to its corresponding string
 * representation.
 *
 * @param key The `Key` enum value.
 * @return A `std::string_view` that corresponds to the given key (e.g., "name",
 * "version").
 */
constexpr std::string_view toString(Key key) noexcept {
  switch (key) {
  case Key::Name:
    return "name";
  case Key::Version:
    return "version";
  case Key::Authors:
    return "authors";
  case Key::Compiler:
    return "compiler";
  case Key::Standard:
    return "standard";
  case Key::BuildType:
    return "build_type";
  }
}
} // namespace constants
