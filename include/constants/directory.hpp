/**
 * @file directory.hpp
 * @brief Defines directory-related constants and utilities.
 *
 * This header provides an enumeration for commonly used directory names
 * within the project structure, as well as a function for converting these
 * enum values to their corresponding string representations.
 */

#pragma once

#include <string_view>

namespace constants {
/**
 * @brief Enumerates common directories used within the project's structure.
 *
 * These directories represent important locations within a typical C++ project,
 * including the current directory, the `include` directory for headers, the
 * `src` directory for source files, and a dedicated `build` directory.
 */
enum class Directory { Current, Include, Src, Build };

/**
 * @brief Converts a `Directory` enumeration value to its corresponding string
 * representation.
 *
 * @param dir The `Directory` enum value.
 * @return A `std::string_view` containing the name of the directory (e.g., ".",
 * "include", "src", or "build").
 */
constexpr std::string_view toString(Directory dir) noexcept {
  switch (dir) {
  case Directory::Current:
    return ".";
  case Directory::Include:
    return "include";
  case Directory::Src:
    return "src";
  case Directory::Build:
    return "build";
  }
}
} // namespace constants
