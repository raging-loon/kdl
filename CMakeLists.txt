cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
    cmake_policy(SET CMP0141 NEW)
    set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

set(CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build")


function(set_source_files varname)

    set(filelist ${ARGV})
    list(REMOVE_AT filelist 0)

    set(adjusted_filelist "")

    foreach(arg IN LISTS filelist)
        string(FIND "${arg}" "${CMAKE_CURRENT_SOURCE_DIR}" pos)
        if(NOT pos EQUAL 0)
            list(APPEND adjusted_filelist "${CMAKE_CURRENT_SOURCE_DIR}/${arg}")
        else()
            list(APPEND adjusted_filelist "${arg}")
        endif()

    endforeach()

    set(${varname} ${adjusted_filelist} CACHE INTERNAL "" FORCE)
endfunction()

add_subdirectory(kdlc)

