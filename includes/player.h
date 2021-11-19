#ifndef MALLOCWORLD_PLAYER_H
#define MALLOCWORLD_PLAYER_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

#include <terminalManager.h>

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

#include <monster.h>

player* createPlayer();
void freePlayer(player *player);

//---------------------- Etat du player ----------------------
bool playerIsDead(player *player);
void playerTakeDamage(player *player, uint16_t amount);
void playerWinExp(player *player, uint16_t exp);
void playerLevelUp(player *player);


//---------------------- Affichage ----------------------
void printPlayer(player *player);
void printLevel(player *player);
void printExp(player *player);
void printLife(player *player);

//|--------------------------------------------| ACTION |--------------------------------------------|
//---------------------- Fight ----------------------
int playerTurnFight(player *player, monster *monster);
int playerDoDamage(player *player, monster *monster);
int playerSwitchWeapon(player *player);
int playerSwitchArmor(player *player);
int playerUsePotion(player *player);
int playerEscape(player *player);
int playerDoChoiceCategory(item **tabItem);

//---------------------- Map ----------------------
void playerMoov(player *player, int ***map, direction direction);
int playerCraft(player *player);
int playerMine(player *player);
int playerChopWood(player *player);
int playerCutGrass(player *player);
int playerCraftItem(player *player, int id);


#endif //MALLOCWORLD_PLAYER_H
