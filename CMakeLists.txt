cmake_minimum_required(VERSION 3.16)
project(Project)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

file(GLOB CPP
	"src/*"
)

add_library(all_cpp OBJECT ${CPP})

add_executable(Project main.cpp)
target_link_libraries(Project all_cpp)
