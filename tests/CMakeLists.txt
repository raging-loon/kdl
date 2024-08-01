cmake_minimum_required (VERSION 3.8)

project(tset C)

add_library(tset SHARED "test.c")

target_include_directories(tset PUBLIC "../../libdetutil/include")
target_link_libraries(tset PUBLIC "../../libdetutil/build/libdetutil")

target_compile_options(tset PRIVATE "$<$<CONFIG:Debug>:/RTC->")
target_compile_options(tset PRIVATE "$<$<CONFIG:Debug>:/Zi>")
target_compile_options(tset PRIVATE "$<$<CONFIG:Debug>:/Zl>")
target_link_options(tset PRIVATE "$<$<CONFIG:Debug>:/NODEFAULTLIB>")
target_link_options(tset PRIVATE "$<$<CONFIG:Debug>:/ENTRY:DllMain>")
target_link_options(tset PRIVATE "$<$<CONFIG:Debug>:/SUBSYSTEM:NATIVE>")
target_link_options(tset PRIVATE "$<$<CONFIG:Debug>:/DRIVER>")