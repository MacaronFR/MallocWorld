#ifndef MALLOCWORLD_GAME_H
#define MALLOCWORLD_GAME_H

#include <player.h>
#include <player.h>
#include <respawn.h>

void startGame();
void endGame();
void fight(player *player, monster *monster, respawn **list, int32_t x, int32_t y);

#endif //MALLOCWORLD_GAME_H
