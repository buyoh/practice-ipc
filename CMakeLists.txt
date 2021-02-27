cmake_minimum_required(VERSION 3.5)
project(nospace CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-O2 -Wall")

include_directories(
  PUBLIC  ${PROJECT_SOURCE_DIR}/src
)

add_subdirectory(src)