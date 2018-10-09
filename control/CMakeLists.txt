cmake_minimum_required(FATAL_ERROR VERSION 3.7.1)

add_definitions(-DEXPORT_FUNC)

set(PROJECT SoundControl)
project(${PROJECT})

add_library(${PROJECT} SHARED SoundControl.hpp SoundControl.cpp)

install(TARGETS ${PROJECT} DESTINATION lib)
install(FILES SoundControl.hpp DESTINATION include)