project(mallocworld C)
cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 99)

include_directories("includes")

#Permet de créer des variable regroupant des fichiers
set(SRCS
        src/main.c
        src/game.c
        src/player.c
        src/inventory.c
        src/item.c
        src/utils.c
        src/monster.c
        src/perlin.c
        src/terminalManager.c
        src/craft.c
        src/resource.c
        src/respawn.c src/save.c)

#Permet l'affichage de debug lors du déroulement du programme
#À commenter pour version release
add_definitions(-DDEBUG -DVERBOSE)

add_executable(mallocworld ${SRCS})

target_link_libraries(mallocworld -lm)