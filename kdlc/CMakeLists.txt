
project(kdlc)
set(CXX_STANDARD 20 REQUIRED)

include(FetchContent)
FetchContent_Declare(
    antlr4_runtime
    GIT_REPOSITORY https://github.com/antlr/antlr4.git
    GIT_TAG 4.13.1
)

FetchContent_MakeAvailable(antlr4_runtime)

message(${antlr4_runtime_SOURCE_DIR})
message("${CMAKE_SOURCE_DIR}/profiling_atn_simulator_cpp_include_chrono.patch")
if (NOT EXISTS "${antlr4_runtime_SOURCE_DIR}/.patched")

    execute_process(  
        COMMAND git.exe apply "${CMAKE_SOURCE_DIR}/profiling_atn_simulator_cpp_include_chrono.patch"
        WORKING_DIRECTORY "${antlr4_runtime_SOURCE_DIR}"

    )

    file(WRITE  "${antlr4_runtime_SOURCE_DIR}/.patched" "patch applied")
    message("patched file")
endif()

add_subdirectory(${antlr4_runtime_SOURCE_DIR}/runtime/Cpp antlr4_runtime_build)

add_executable(kdlc
    main.cpp
)


target_link_libraries(kdlc
PRIVATE
    antlr4_static

)
