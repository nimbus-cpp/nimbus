cmake_minimum_required(VERSION 3.15)
project(nimbus CXX)

set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_STANDARD 23)

find_package(CLI11 REQUIRED)

include_directories(include)

add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} CLI11::CLI11)
