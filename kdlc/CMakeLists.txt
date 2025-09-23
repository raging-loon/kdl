project(kdlc)


set(CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD 20)


add_executable(kdlc
    main.cpp
 "parser/Token.h" "parser/Token.cpp")