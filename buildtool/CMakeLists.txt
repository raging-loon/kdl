cmake_minimum_required (VERSION 3.8)

project(%name% C)

set(C_SOURCES %sources%)

option(KSR_DYNAMIC_LIBRARY "Compile as a dynamic (dll/so) library" OFF)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY %outputdirectory%)

add_library(%name% 
	SHARED 
	${C_SOURCES}
)


if(KSR_DYNAMIC_LIBRARY)
	add_compile_definitions(KSR_COMPILE_AS_DYNLIB=0)
endif()

target_include_directories(%name%
	PUBLIC 
	%location_of_libdetutil_includes%
)
target_link_libraries(
	%name% 
	PUBLIC 
	%location_of_libdetutil_staticlib%
	
)

