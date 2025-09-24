project(kdlc)


set(CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD 20)

add_subdirectory(parser)
add_subdirectory("interface")
add_subdirectory("context")

add_executable(kdlc
    main.cpp
    ${PARSER_SOURCES}
    ${INTERFACE_SOURCES}
    ${CONTEXT_SOURCES}
)

target_include_directories(kdlc
PRIVATE
    "./"
)