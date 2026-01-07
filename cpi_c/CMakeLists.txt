cmake_minimum_required(VERSION 3.28)

project(
  CambridgePseudocodeInterpreter
  VERSION 0.1
  LANGUAGES C)

option(CPI_BUILD_TESTS "Build tests" ON)
option(CPI_BUILD_MAIN "Build the program" ON)

if (MSVC)
  add_compile_options(
    /Wall
    /wd4820 # Padding
    /wd4710 # Function was not inlined
    /wd5045 # Spectre mitigations
    /wd4514 # Unreferenced inline function has been removed
    /wd4711 # Function selected for automatic inlining
    /wd4706 # Assignment inside conditional expression (even with double paren)
    /wd4061 # Non-explicitly handled enum value (Doesn't count `default:')
    /wd4623 # Default constructor was implicitly defined as deleted
    /wd4868 # Compiler may not enforce left-to-right evaluation order in braced initializer list
    /wd4127 # Conditional expression is constant (Even if block contains a `break')
    /wd4625 # Copy constructor was implicitly defined as deleted
    /wd4626 # Assignment operator was implicitly defined as deleted
    /wd4574 # Macro is defined to be '0': did you mean to use '#if'?
    /wd4100 # Unreferenced formal parameter
    -D_CRT_SECURE_NO_WARNINGS # Annex K functions are just fine.
  )
else()
  add_compile_options(-Wall -Wextra -pedantic)
endif()

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