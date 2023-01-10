set(PREFIX base-oo)


add_library(${PREFIX}-call_smoke SHARED strategy/call_smoke.cpp)
add_library(${PREFIX}-call_phone SHARED strategy/call_phone.cpp)
add_library(${PREFIX}-maybe_throw_strategy SHARED strategy/maybe_throw_strategy.cpp)

add_executable(${PREFIX}-factory-main main.cpp)

target_link_libraries(${PREFIX}-factory-main PRIVATE "-Wl,--no-as-needed"
	${PREFIX}-call_smoke ${PREFIX}-call_phone ${PREFIX}-maybe_throw_strategy)

