cmake_minimum_required(VERSION 3.17)
project(Project)

set(CMAKE_CXX_STANDARD 17)
include_directories(include)
find_package(Protobuf CONFIG REQUIRED)

find_package(gRPC CONFIG REQUIRED)
find_package(Protobuf REQUIRED)
set(_PROTOBUF_LIBPROTOBUF protobuf::libprotobuf)
set(_REFLECTION gRPC::grpc++_reflection)
message(STATUS "Using gRPC ${gRPC_VERSION}")
include_directories("${CMAKE_CURRENT_BINARY_DIR}")
add_executable(Project main.cpp  src/Wagon_card.cpp src/Deck.cpp  src/Route.cpp  src/Path.cpp  src/Discharge.cpp  src/Board.cpp  src/Player.cpp  src/Game.cpp )
target_link_libraries(${PROJECT_NAME} PRIVATE
        protobuf::libprotobuf
        gRPC::grpc++
        )