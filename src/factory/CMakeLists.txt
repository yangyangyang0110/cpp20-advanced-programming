add_executable(factory-main main.cpp)

add_library(call_smoke SHARED strategy/call_smoke.cpp)
add_library(call_phone SHARED strategy/call_phone.cpp)

target_link_libraries(factory-main PRIVATE call_smoke call_phone)


