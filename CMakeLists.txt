cmake_minimum_required(VERSION 3.17)
project(Project)

set(CMAKE_CXX_STANDARD 17)

add_executable(Project main.cpp src/Wagon_card.cpp include/Wagon_card.h src/Deck.cpp include/Deck.h src/Route.cpp include/Route.h src/Path.cpp include/Path.h src/Discharge.cpp include/Discharge.h src/Board.cpp include/Board.h src/Player.cpp include/Player.h src/Game.cpp include/Game.h)