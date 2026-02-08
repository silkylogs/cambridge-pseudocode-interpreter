cmake_minimum_required(VERSION 3.28.3)

option(CPI_BUILD_TESTS "Build tests" ON)
option(CPI_BUILD_MAIN "Build the program" ON)

if (false)
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
        /ZI # Hot reloading
      )
      add_link_options(
        /INCREMENTAL # Hot reloading
      )
    elseif (GNU)
      add_compile_options(
        -Wall -Wextra -pedantic
        -Wwrite-strings
        -Wstrict-prototypes
        -Wmissing-prototypes
        -Wmissing-declarations
        -Wsign-compare
        -Wlogical-op
        -Wtype-limits
        -Wsuggest-attribute=pure
        -Wsuggest-attribute=const
        -Wsuggest-attribute=noreturn
        -Wsuggest-attribute=format
        -Wformat-nonliteral
        -Werror=vla
        -g
      )
    elseif (Clang)
      add_compile_options(-Wmost)
    else () # Appleclang, I suppose
      add_compile_options(-Wmost)
    endif()
endif()

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

#if(CPI_BUILD_TESTS)
#    add_executable(test)
#    target_sources(test PRIVATE test.c)
#    if (MSVC)
#        target_compile_options(test PRIVATE /ZI)
#    endif ()
#    target_link_libraries(test PRIVATE Vm)
#endif()
#
#if(CPI_BUILD_MAIN)
#    add_executable(cpi)
#    target_sources(cpi PRIVATE main.c)
#    target_link_libraries(cpi PRIVATE Vm)
#endif()

