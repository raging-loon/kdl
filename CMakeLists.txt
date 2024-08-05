# CMakeList.txt : CMake project for libkdl, src source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

set(CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD 20)

project ("kdlc")

set(LIBKDL_DIR ${CMAKE_SOURCE_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build")



# Add source to this project's executable.
add_executable(kdlc   "src/parser/lex.h" 
					   "src/parser/tokens.h" "src/parser/lex.cpp" "src/utils.h" 
					   "src/parser/tokens.cpp" "src/parser/parser.h" "src/parser/parser.cpp" 
					   "src/channels/rule.h"  "src/channels/type.h" "src/parser/RuleParser.h"
					   "src/parser/RuleParser.cpp"  "src/parser/Combinator.h"
					   "src/parser/Combinator.cpp" "src/channels/RulePool.h" 
					   "src/channels/rule.cpp" "src/utils.cpp" "src/conditional/ConditionalTree.h" 
					   "src/conditional/ConditionalParser.h" "src/conditional/ConditionalParser.cpp" "src/conditional/ConditionalTree.cpp" 
					   "src/conditional/ConditionalSemanticChecker.h" "src/conditional/ConditionalSemanticChecker.cpp" 
					   "src/parser/CompilerMessage.h" 
					   "src/parser/CompilerMessage.cpp" "src/backend/CodeGenerator.h" "src/search/search.h"
					   "src/conditional/ConditionalReferenceValidator.h" "src/Compiler.h" "src/Compiler.cpp" 
					   "src/conditional/ConditionalReferenceValidator.cpp"  "src/template/Array.h"  
					   "src/backend/clang/CLangCG.h" "src/backend/clang/CLangCG.cpp" 
					   "src/backend/clang/ClRuleCompiler.h" "src/backend/clang/ClRuleCompiler.cpp" "src/kdlc.cpp" "src/cli/TerminalInterface.h" "src/cli/TerminalInterface.cpp" "src/kdlc.h" "src/cli/KdlOptions.h" "src/cli/KdlOptions.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET kdlc PROPERTY CXX_STANDARD 20)
endif()
if (MSVC)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} setargv.obj")
endif ()
# TODO: Add tests and install targets if needed.

# TODO: add a kestrel directory variable
target_include_directories(
	kdlc 
	PUBLIC 
	"./src" )


