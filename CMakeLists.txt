project(mallocworld C)
cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 99)

include_directories("includes")

#Permet l'affichage de debug lors du déroulement du programme
#À commenter pour version release
add_definitions(-DDEBUG -DVERBOSE)

add_executable(mallocworld src/main.c src/player.c src/inventory.c src/utils.c src/perlin.c src/item.c src/monster.c)
target_link_libraries(mallocworld -lm)