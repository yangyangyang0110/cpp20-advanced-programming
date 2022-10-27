file(GLOB FILES *.cpp)
dynamic_build_execute(template ${FILES})

file(GLOB FILES singular-recursive-template/*.cpp)
dynamic_build_execute(template-srt ${FILES})

