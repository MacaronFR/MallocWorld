project(mallocworld C)
cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 99)

include_directories("includes")

add_executable(mallocworld src/main.c src/testPlayer.c src/player.c src/inventory.c src/map.c)
