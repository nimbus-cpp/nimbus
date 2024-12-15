/// Comment

#pragma once

#include <string_view>

namespace constants {

/// Comment
enum class Directory { Current, Include, Src, Build };

/// Comment
constexpr std::string_view toString(Directory dir) {
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
