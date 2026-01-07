cmake_minimum_required(VERSION 3.28)

project(
  CambridgePseudocodeInterpreter
  VERSION 0.1
  LANGUAGES C)

option(CPI_BUILD_TESTS "Build tests" ON)
option(CPI_BUILD_MAIN "Build the program" ON)

add_library(Vm STATIC)
target_sources(Vm PRIVATE vm.c vm.h)

if(CPI_BUILD_TESTS)
    add_executable(test)
    target_sources(test PRIVATE test.c)
    target_link_libraries(test PRIVATE Vm)
endif()

if(CPI_BUILD_MAIN)
    add_executable(cpi)
    target_sources(cpi PRIVATE main.c)
    target_link_libraries(cpi PRIVATE Vm)
endif()