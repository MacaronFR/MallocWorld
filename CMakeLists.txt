project(mallocworld C)

set(CMAKE_C_STANDARD 99)

add_executable(mallocworld src/main.c src/mainPlayer.c src/player.c src/inventory.c)