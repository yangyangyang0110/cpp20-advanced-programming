add_library(some_func SHARED)
target_sources(some_func PUBLIC some.cpp)

add_executable(some_main main.cpp)
target_link_libraries(some_main some_func)


