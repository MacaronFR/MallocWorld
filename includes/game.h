#ifndef MALLOCWORLD_GAME_H
#define MALLOCWORLD_GAME_H

#include <player.h>
#include <player.h>
#include <respawn.h>


void playerTurn(player *player, int ***map);



void startGame();
void endGame();


void fight(player *player, monster *monster, respawn **list, int32_t x, int32_t y, int8_t lvl);

// --------------------------------- AFFICHAGE ---------------------------------
void printStartMenu();
void printPlayerInterface();
void printCredit();

#endif //MALLOCWORLD_GAME_H
