
project(kdlc)
set(CXX_STANDARD 20 REQUIRED)

add_compile_definitions(ANTLR4CPP_STATIC)
include(FetchContent)
FetchContent_Declare(
    antlr4_runtime
    GIT_REPOSITORY https://github.com/antlr/antlr4.git
    GIT_TAG 4.13.1
)

FetchContent_MakeAvailable(antlr4_runtime)
if (win32)
    if (NOT EXISTS "${antlr4_runtime_SOURCE_DIR}/.patched")
    
        execute_process(  
            COMMAND git apply "${CMAKE_SOURCE_DIR}/profiling_atn_simulator_cpp_include_chrono.patch"
            WORKING_DIRECTORY "${antlr4_runtime_SOURCE_DIR}"
    
        )
    
        file(WRITE  "${antlr4_runtime_SOURCE_DIR}/.patched" "patch applied")
        message("patched file")
    endif()
endif()

add_subdirectory(${antlr4_runtime_SOURCE_DIR}/runtime/Cpp antlr4_runtime_build)

set(ANTLR4CPP_JAR_LOCATION "${CMAKE_SOURCE_DIR}/thirdparty/antlr/antlr-4.13.2-complete.jar")
set(GRAMMAR "${CMAKE_CURRENT_SOURCE_DIR}/grammar/KDLGrammar.g4")
set(GENERATE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/generated")
set(COMMAND_OUTPUT "${GENERATE_DIR}/KDLGrammarLexer.cpp" "${GENERATE_DIR}/KDLGrammarVisitor.cpp")

add_custom_command(
    OUTPUT
        ${COMMAND_OUTPUT}
    COMMAND 
        ${CMAKE_COMMAND} -E make_directory ${GENERATE_DIR}
    COMMAND 
        java -jar ${ANTLR4CPP_JAR_LOCATION} -o ${GENERATE_DIR} -Dlanguage=Cpp ${GRAMMAR} -package kdl_gen -visitor

    DEPENDS
        ${GRAMMAR}
    VERBATIM
)
message(${GRAMMAR})
add_custom_target(
    antlr4_generate
    DEPENDS
        ${COMMAND_OUTPUT}
)

file(GLOB GENERATED_SRCS "${GENERATE_DIR}/*.cpp" )

add_subdirectory(ast)
add_subdirectory(context)
add_subdirectory(interface)

add_executable(kdlc
    main.cpp
    ${AST_SRCS}
    ${CTX_SOURCES}
    ${GENERATED_SRCS}
    ${INTERFACE_SRCS}
)
add_dependencies(kdlc antlr4_generate)
target_include_directories(kdlc
PRIVATE
    ${antlr4_runtime_SOURCE_DIR}/runtime/Cpp/runtime/src
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(kdlc
PRIVATE
    antlr4_static

)
