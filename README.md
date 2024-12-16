# Nimbus: A Modern C++ Build System and Package Manager

**Nimbus** is a lightweight and modern C++ build system and package manager designed to streamline your C++ development workflow. With a simple command-line interface, easy configuration through `nimbus.toml`, and intelligent handling of build options, Nimbus helps you focus on writing code rather than wrestling with build files.

## Features

- **Initialize New Projects:** Quickly set up a new C++ project with sensible defaults using `nimbus init`. It automatically creates a source and include directory, generates a `nimbus.toml` configuration file.
  
- **Build with Ease:** Compile your project using `nimbus build`. Nimbus leverages your compiler of choice and the settings defined in `nimbus.toml` to create an optimized or debug-friendly binary, depending on your specified build type.
  
- **Intelligent Build Types:** Select from various build types such as `Debug`, `Release`, `RelWithDebInfo`, or `MinSizeRel`. Nimbus automatically applies the corresponding compiler flags and **preprocessor macros** (e.g., `DEBUG`, `RELEASE`, `RELWITHDEBINFO`, `MINSIZEREL`) at compile time, enabling or disabling certain debugging features, optimization levels, or size-reduction strategies as needed.
  
- **Flexible Configuration:** Control your project’s compiler, standard, and build type directly from `nimbus.toml`. Easily tweak settings without touching build scripts, thanks to an intuitive TOML-based configuration.

## Getting Started

1. **Installation (Ensure you have a clang++/g++ supporting modules, as well as toml11 and CLI11 on your machine):**  

   ```bash
   git clone https://github.com/nimbus-cpp/nimbus.git
   cd nimbus
   mkdir build && cd build

   # init module
   clang++ -std=c++20 ../src/commands/init.cppm --precompile -o init.pcm -I/path/to/includes/CLI11+toml11
   clang++ -std=c++20 ../src/commands/init.cppm -c -o init.o -I/path/to/includes/CLI11+toml11

   # build module
   clang++ -std=c++20 ../src/commands/build.cppm --precompile -o build.pcm -I/path/to/includes/CLI11+toml11
   clang++ -std=c++20 ../src/commands/build.cppm -c -o build.o -I/path/to/includes/CLI11+toml11

   # main
   clang++ -std=c++20 ../src/main.cpp -fprebuilt-module-path=. init.o build.o -o nimbus -I/path/to/includes/CLI11+toml11
   ```
3. **Project initialization:**

   To initialize the current directory as a nimbus project (which will use the dir name as the name of the project in the generated `nimbus.toml`):
   ```bash
   nimbus init
   ```
   To create a new project from scratch:
   ```bash
   nimbus init my_project
   ```
   This sets up a basic directory structure:
   ```css
   my_project/
   ├─ include/
   ├─ src/
   └─ nimbus.toml
   ```
   Inside nimbus.toml, you'll find default values for your project's name, version, authors, and build configuration:
   ```toml
   [project]
   name = "my_project"
   version = "0.1.0"
   authors = ["Your Name <you@example.com>"]

   [build]
   compiler = "clang++"
   standard = "c++20"
   build_type = "Debug"
   ```
   Adjust these as needed. For example, changing build_type from "Debug" to "Release" switches your project from including debugging symbols and `DEBUG` macro to producing an optimized binary with the `RELEASE` preprocessing macro.
4. **Building the project:**

   Once your project is initialized and configured, running:
   ```bash
   nimbus build
   ```
   Will:
   - Create the `build` directory if it doesn’t exist.
   - Read configuration options from `nimbus.toml`.
   - Compile all `.cpp`, `.c`, or `.cxx` files in `src/` using the specified compiler and flags.
   - Apply the appropriate preprocessor definitions based on the selected build type.
   - Generate the output binary in `build/`, naming it using the name config from `nimbus.toml`.

   Supported `build_type` values and their generated preprocessor macros(available to use in-code):
   - Debug -> DEBUG
   - Release -> RELEASE
   - RelWithDebInfo -> RELWITHDEBINFO
   - MinSizeRel -> MINSIZEREL

   ```cpp
   #include <iostream>

   int main() {
   #ifdef DEBUG
     std::cout << "Build Mode: Debug\n";
   #elif defined(RELEASE)
     std::cout << "Build Mode: Release\n";
   #elif defined(RELWITHDEBINFO)
     std::cout << "Build Mode: RelWithDebInfo\n";
   #elif defined(MINSIZEREL)
     std::cout << "Build Mode: MinSizeRel\n";
   #else
     std::cout << "Build Mode: Unknown\n";
   #endif
     return 0;
   }
   ```
