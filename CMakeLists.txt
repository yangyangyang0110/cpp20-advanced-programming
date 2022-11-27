cmake_minimum_required(VERSION 3.23 FATAL_ERROR)

project(cpp20-advanced-programming VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

### Find Deps
#find_package(spdlog REQUIRED)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(spdlog)
include(json)
include(gtest)
# conan
include(range-v3/conanbuildinfo)
conan_basic_setup(TARGETS) # Introduce Conan-generated targets


function(dynamic_build_execute prefix)
  foreach (example ${ARGN})
    get_filename_component(target ${example} NAME_WLE)
    string(FIND ${example} "_test.cpp" FIND_RESULT)
    if (${FIND_RESULT} STREQUAL "-1")
      set(IS_TEST_TARGET FALSE)
    else ()
      set(IS_TEST_TARGET TRUE)
    endif ()
    
    if (NOT ${prefix} STREQUAL "")
      set(target "${prefix}-${target}")
    endif ()
    string(REPLACE "-" "_" target ${target})
    
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
      nlohmann_json::nlohmann_json
      $<$<BOOL:${IS_TEST_TARGET}>:GTest::gtest_main>
      CONAN_PKG::range-v3
    )
    
    if (${IS_TEST_TARGET})
      message(STATUS "Is test ${target}")
      include(GoogleTest)
      gtest_discover_tests(${target})
    endif ()
  
  endforeach ()
endfunction()

include_directories(src)
add_subdirectory(src)

enable_testing()
#add_subdirectory(test)

