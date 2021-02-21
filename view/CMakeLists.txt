cmake_minimum_required(VERSION 3.16)
project(test_qt)

set(CMAKE_CXX_STANDARD 17)

file(GLOB CPP
	"src/*"
)

add_library(QT_VIEW ${CPP})
target_include_directories(QT_VIEW PUBLIC include)

#add_library(all_cpp OBJECT ${CPP})

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} QT_VIEW)
