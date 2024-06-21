cmake_minimum_required(VERSION 3.0)

set (CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_C_COMPILER "gcc")


# argparse library
add_subdirectory(${CMAKE_SOURCE_DIR}/lib/argparse)
include_directories(${CMAKE_SOURCE_DIR}/lib/argparse/include)

# rapdidxml library
include_directories(${CMAKE_SOURCE_DIR}/lib/rapidxml)

# GDI+ library
set(GDIPLUS_LIB_PATH ${CMAKE_SOURCE_DIR}/lib/gdiplus)
find_library(GDIPLUS_LIB gdiplus PATHS ${GDIPLUS_LIB_PATH})
if(NOT GDIPLUS_LIB)
    message(FATAL_ERROR "libgdiplus.a not found. Please check the path.")
endif()

# base64 library
#add_subdirectory(${CMAKE_SOURCE_DIR}/lib/base64)
include_directories(${CMAKE_SOURCE_DIR}/lib/base64/include)

project(PosPrinterModule)

# include directories
include_directories(${CMAKE_SOURCE_DIR}/includes)

file(GLOB_RECURSE MAIN_FILE "${CMAKE_SOURCE_DIR}/src/*.cpp")
#set(MAIN_FILE "${CMAKE_SOURCE_DIR}/src/main.cpp")


set(CMAKE_EXE_LINKER_FLAGS "-static")


# Create executable
add_executable(PosPrinterModule ${MAIN_FILE})
set_target_properties(PosPrinterModule PROPERTIES OUTPUT_NAME "pos_print")
target_link_libraries(PosPrinterModule argparse ${GDIPLUS_LIB})
#add_dependencies(PosPrinterModule argparse ${GDIPLUS_LIB})