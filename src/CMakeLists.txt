cmake_minimum_required(VERSION 3.28)

file(GLOB_RECURSE src CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

# Make these only be included in debug builds
add_compile_definitions(FCONFIG_MEMORY_TRACKING VERBOSE_CHUNK_TRACKING)

add_executable(aoc "${src}")

target_link_libraries(aoc foundation)

target_include_directories(aoc PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")