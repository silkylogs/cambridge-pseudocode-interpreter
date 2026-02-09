cmake_minimum_required(VERSION 3.28.3)

project(cpi_cpp)
add_executable(cpi_cpp src/main.cpp src/util.cpp)
target_compile_features(cpi_cpp PUBLIC cxx_std_23)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
set_target_properties(cpi_cpp PROPERTIES CXX_EXTENSIONS OFF)
target_link_libraries(cpi_cpp)
