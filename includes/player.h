#ifndef MALLOCWORLD_PLAYER_H
#define MALLOCWORLD_PLAYER_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

#include <terminalManager.h>
#include <monster.h>
#include <inventory.h>
#include <item.h>
#include <utils.h>

typedef enum e_experience {
    LEVEL1 = 10,
    LEVEL2 = 20,
    LEVEL3 = 30,
    LEVEL4 = 40,
    LEVEL5 = 50,
    LEVEL6 = 60,
    LEVEL7 = 70,
    LEVEL8 = 80,
    LEVEL9 = 90,
    LEVEL10 = 100
}experience;

typedef struct s_stuff {
	item* armor;
	item* weapon;
}stuff;

typedef struct s_player {
    inventory *inventory;
    uint16_t life;
    uint16_t maxLife;
    uint16_t exp;
    uint8_t level;
	stuff *stuff;
	coordinate *coordinate;
}player;

player* createPlayer();
void initPlayer(player *player);
void freePlayer(player *player);

//---------------------- Etat du player ----------------------
bool playerIsDead(player *player);
void playerTakeDamage(player *player, int amount);

void playerWinExp(player *player, uint16_t exp);
void playerLevelUp(player *player);



item* getArmor(player *player, int id);
item* getWeapon(player *player, int id);
item* getRessource(player *player, int id, int nb);

//---------------------- Affichage ----------------------
void printPlayer(player *player);
void printLevel(player *player);
void printExp(player *player);
void printLife(player *player);

//---------------------- ACTION ----------------------
int playerTurnFight(player *player, monster *monster);
int playerDoDamage(player *player, monster *monster);
int playerSwitchWeapon(player *player);
int playerSwitchArmor(player *player);
int playerUsePotion(player *player);
int playerEscape(player *player);

int playerDoChoiceCategory(item **tabItem);

#endif //MALLOCWORLD_PLAYER_H
