cmake_minimum_required(VERSION 3.28.3)

project(cpi_cpp)
add_executable(cpi_cpp src/main.cpp)
target_compile_features(cpi_cpp PUBLIC cxx_std_23)
set_target_properties(cpi_cpp PROPERTIES CXX_EXTENSIONS OFF)

# https://stackoverflow.com/questions/17511496/how-to-create-a-shared-library-with-cmake
project(ForthVM)
add_library(ForthVM STATIC src/forth.cpp) # https://stackoverflow.com/questions/40739061/error-lnk1104-cannot-open-file-debug-myprojectlib-lib
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_features(ForthVM PUBLIC cxx_std_23)

target_link_libraries(cpi_cpp ForthVM)
