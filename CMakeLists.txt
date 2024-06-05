cmake_minimum_required(VERSION 3.0)

set (CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_C_COMPILER "gcc")


# argparse library
add_subdirectory(${CMAKE_SOURCE_DIR}/lib/argparse)
include_directories(${CMAKE_SOURCE_DIR}/lib/argparse/include)
include_directories(${CMAKE_SOURCE_DIR}/lib/rapidxml)

project(PosPrinterModule)

# include directories
include_directories(${CMAKE_SOURCE_DIR}/includes)

file(GLOB_RECURSE MAIN_FILE "${CMAKE_SOURCE_DIR}/src/*.cpp")
#set(MAIN_FILE "${CMAKE_SOURCE_DIR}/src/main.cpp")

set(CMAKE_EXE_LINKER_FLAGS "-static")
# Create executable
add_executable(PosPrinterModule ${MAIN_FILE})
set_target_properties(PosPrinterModule PROPERTIES OUTPUT_NAME "pos_print")
add_dependencies(PosPrinterModule argparse)