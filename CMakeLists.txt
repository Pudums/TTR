cmake_minimum_required(VERSION 3.16)
project(Project)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)
file(GLOB CPP
	"src/*"
)

find_package(Protobuf CONFIG REQUIRED)

find_package(gRPC CONFIG REQUIRED)
find_package(Protobuf REQUIRED)
set(_PROTOBUF_LIBPROTOBUF protobuf::libprotobuf)
set(_REFLECTION gRPC::grpc++_reflection)
message(STATUS "Using gRPC ${gRPC_VERSION}")
include_directories("${CMAKE_CURRENT_BINARY_DIR}")
add_library(all_cpp OBJECT ${CPP})
add_executable(Project main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE
        protobuf::libprotobuf
        gRPC::grpc++
        )
file(GLOB CC "protobuf/*.pb.*")
add_library(proto OBJECT ${CC})
target_link_libraries(Project PUBLIC all_cpp proto)
