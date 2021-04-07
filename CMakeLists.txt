cmake_minimum_required(VERSION 3.16)

project(Project)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

file(GLOB CPP
	"src/*"
	"src/Controller/*"
	"src/View/*"
	"src/Server/*"
	"src/Server/protobuf/*"
)

file(GLOB H
	"include/*"
	"include/Controller/*"
	"include/View/*"
	"include/Server/*"
	"include/Server/protobuf/*"
)

find_package(
  Qt5
  COMPONENTS Quick
             QuickCompiler
             Widgets
             Gui
  REQUIRED)
find_package(Protobuf CONFIG REQUIRED)

find_package(gRPC CONFIG REQUIRED)
find_package(Protobuf REQUIRED)
set(_PROTOBUF_LIBPROTOBUF protobuf::libprotobuf)
set(_REFLECTION gRPC::grpc++_reflection)

file(GLOB CC "include/Server/protobuf/*.pb.*")
add_library(proto OBJECT ${CC})
add_library(QT_VIEW ${CPP} ${H})
target_include_directories(proto PUBLIC include include/Server)
target_include_directories(QT_VIEW PUBLIC include include/View include/Controller)
target_link_libraries(QT_VIEW
	PUBLIC
	Qt5::Quick
    Qt5::Gui
    Qt5::Widgets
    Qt5::Core
)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(proto
		${_REFLECTION}
		${_GRPC_GRPCPP}
		${_PROTOBUF_LIBPROTOBUF})

target_link_libraries(${PROJECT_NAME} QT_VIEW)
target_link_libraries(${PROJECT_NAME} proto)
