cmake_minimum_required(VERSION 3.23 FATAL_ERROR)

project(cpp20-advanced-programming VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

### Find Deps
#find_package(spdlog REQUIRED)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(spdlog)


function(dynamic_build_execute prefix)
    foreach (example ${ARGN})
        get_filename_component(target ${example} NAME_WLE)
        string(REPLACE "_" "-" target ${target})
        if (NOT ${prefix} STREQUAL "")
            set(target "${prefix}-${target}")
        endif ()
        message(STATUS "build ${example} to ${target}...")
        add_executable(${target} ${example})
        
        target_compile_options(
            ${target}
            PRIVATE
            -Werror -fPIC
            $<$<CONFIG:Debug>:-g -O0>
            $<$<CONFIG:Release>:-O2>
            #                -ftemplate-depth=10000000
        )
        
        target_compile_definitions(
            ${target}
            PRIVATE
            $<$<CONFIG:Debug>:MyDebug>
        )
        
        target_link_libraries(
            ${target}
            PRIVATE
            pthread
            spdlog::spdlog
        )
    endforeach ()
endfunction()

include_directories(src)
add_subdirectory(src)
