/// Comment

#pragma once

#include <string_view>

namespace constants {

/// Comment
constexpr std::string_view ConfigFile = "nimbus.toml";

// TODO: Automate using existing enum constants!
/// Comment
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

/// Comment
enum class Table { Project, Build };

/// Comment
constexpr std::string_view toString(Table tbl) noexcept {
  switch (tbl) {
  case Table::Project:
    return "project";
  case Table::Build:
    return "build";
  }
}

/// Comment
enum class Key { Name, Version, Authors, Compiler, Standard, BuildType };

/// Comment
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
