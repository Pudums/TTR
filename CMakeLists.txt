cmake_minimum_required(VERSION 3.17)
project(Project)

set(CMAKE_CXX_STANDARD 17)
include_directories(include)
add_executable(Project main.cpp src/Wagon_card.cpp src/Deck.cpp  src/Route.cpp  src/Path.cpp  src/Discharge.cpp  src/Board.cpp  src/Player.cpp  src/Game.cpp )