include(FetchContent)


FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.10.0
)
FetchContent_MakeAvailable(spdlog)

#FetchContent_GetProperties(spdlog)
#if (NOT spdlog_POPULATED)
#    FetchContent_Populate(spdlog)
#    add_subdirectory(${spdlog_SOURCE_DIR} ${spdlog_BINARY_DIR})
#endif ()




