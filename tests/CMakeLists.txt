cmake_minimum_required (VERSION 3.8)

project(tset C)

add_library(tset SHARED "test.c")

add_compile_definitions(KSR_COMPILE_AS_DYNLIB=0)
target_include_directories(tset PUBLIC "../../libdetutil/include")
target_link_libraries(tset PUBLIC "../../libdetutil/build/libdetutil")

