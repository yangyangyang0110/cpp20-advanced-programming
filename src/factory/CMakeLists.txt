add_library(call_smoke SHARED strategy/call_smoke.cpp)
add_library(call_phone SHARED strategy/call_phone.cpp)
add_library(maybe_throw_strategy SHARED strategy/maybe_throw_strategy.cpp)

add_executable(factory-main main.cpp)
target_link_libraries(factory-main PRIVATE "-Wl,--no-as-needed"
	call_smoke call_phone maybe_throw_strategy)



