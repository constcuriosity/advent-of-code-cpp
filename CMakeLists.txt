cmake_minimum_required(VERSION 3.28)

set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE FILEPATH "Attempt to set vcpkg path")

project(aoc)

add_subdirectory("externals/foundation")
add_subdirectory(src)
