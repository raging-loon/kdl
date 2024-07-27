# CMakeList.txt : CMake project for libkdl, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

set(CXX_STANDARD 20)

project ("libkdl")

# Add source to this project's executable.
add_executable (libkdl "lib/libkdl.cpp" "include/libkdl.h" "include/parser/lex.h" 
					   "include/parser/tokens.h" "lib/parser/lex.cpp" "include/utils.h" 
					   "lib/parser/tokens.cpp" "include/parser/parser.h" "lib/parser/parser.cpp" 
					   "include/channels/rule.h"  "include/channels/type.h" "include/parser/RuleParser.h"
					   "lib/parser/RuleParser.cpp"  "include/parser/Combinator.h"
					   "lib/parser/Combinator.cpp"
 "include/channels/RulePool.h" "lib/channels/rule.cpp" "lib/utils.cpp" "include/conditional/ConditionalTree.h" "include/conditional/ConditionalParser.h" "lib/conditional/ConditionalParser.cpp" "lib/conditional/ConditionalTree.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET libkdl PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.

target_include_directories(libkdl PUBLIC "./include")