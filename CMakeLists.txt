cmake_minimum_required(VERSION 3.10)

project(Project)

set(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

file(GLOB CPP
		"src/*"
		"src/Controller/*"
		"src/View/*"
		)

file(GLOB H
		"include/*"
		"include/Controller/*"
		"include/View/*"
		)

find_package(
		Qt5
		COMPONENTS Quick
		#QuickCompiler
		Widgets
		Gui
		REQUIRED)

add_library(QT_VIEW ${CPP} ${H})
target_include_directories(QT_VIEW PUBLIC include include/View include/Controller)

target_link_libraries(QT_VIEW
		PUBLIC
		Qt5::Quick
		Qt5::Gui
		Qt5::Widgets
		Qt5::Core
		)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} QT_VIEW)
