#ifndef MALLOCWORLD_MAP_H
#define MALLOCWORLD_MAP_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#include <player.h>

typedef struct s_map {
    int id;
    int** stage;
    int time;
    player* player;
}map;

map* createMap();
void initMap(map* map);
void freeMap(map* map);
bool endOfGame(map* map);

bool canMine(map* map, player* player);
bool canChopWood(map* map, player* player);
bool canCutGrass(map* map, player* player);
bool canCraftItem(map* map, player* player, int id);

void interactWithPNJ(map* map, player* player);
item* craftItem(player* player, int id);

#endif //MALLOCWORLD_MAP_H