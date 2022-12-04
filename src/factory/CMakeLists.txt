add_executable(factory-main main.cpp
  strategy/call_phone.cpp strategy/call_smoke.cpp
  strategy/maybe_throw_strategy.cpp)
#target_link_libraries(factory-main)

#add_executable(factory-main main.cpp)
#add_library(call_smoke SHARED strategy/call_smoke.cpp)
#add_library(call_phone SHARED strategy/call_phone.cpp)
#target_link_libraries(factory-main PRIVATE call_smoke call_phone)


