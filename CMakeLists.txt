cmake_minimum_required(VERSION 3.15)
project(nimbus CXX)

set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_STANDARD 23)

find_package(CLI11 REQUIRED)
find_package(toml11 REQUIRED)

include_directories(include)

file(GLOB SRC_FILES "src/*.cpp")
add_executable(${PROJECT_NAME} ${SRC_FILES})

target_link_libraries(${PROJECT_NAME} CLI11::CLI11 toml11::toml11)
