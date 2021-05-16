cmake_minimum_required(VERSION 3.16)

project(Project)

set(CMAKE_CXX_STANDARD 17)

#add_compile_options(-fsanitize=address -fsanitize=undefined)
#add_link_options(-fsanitize=address -fsanitize=undefined)

set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
find_package(gRPC CONFIG REQUIRED)
find_package(protobuf CONFIG REQUIRED)

file(GLOB CPP
		"src/*"
		"src/Controller/*"
		"src/View/*"
		"src/Server/*"
		"src/Server/protobuf/*"
        "protobuf/*"
		)

file(GLOB H
		"include/*"
		"include/Controller/*"
		"include/View/*"
		"include/Server/*"
		"include/Server/protobuf/*"
        "protobuf/*"
		)

find_package(
		Qt5
		COMPONENTS Quick
		QuickCompiler
		Widgets
		Gui
		REQUIRED)

add_library(QT_VIEW ${CPP} ${H})
target_include_directories(QT_VIEW PUBLIC include include/View include/Controller protobuf/)
target_link_libraries(QT_VIEW
		PUBLIC
		Qt5::Quick
		Qt5::Gui
		Qt5::Widgets
		Qt5::Core
        gRPC::gpr gRPC::upb gRPC::grpc gRPC::grpc++
        protobuf::libprotoc protobuf::libprotobuf protobuf::libprotobuf-lite
        re2::re2 c-ares::cares
		)
add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} QT_VIEW)

