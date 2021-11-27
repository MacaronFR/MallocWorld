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
#include <perlin.h>
#include <monster.h>
#include <perlin.h>

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
	coordinate abs_coord;
	coordinate relative_coord;
}player;



player* createPlayer(item **listItem, size_t nItem);
void freePlayer(player *player);

//---------------------- Etat du player ----------------------
bool playerIsDead(player *player);
void playerTakeDamage(player *player, uint16_t amount);
void playerWinExp(player *player, uint16_t exp);
void playerLevelUp(player *player);




//|--------------------------------------------| ACTION |--------------------------------------------|
//---------------------- Fight ----------------------
void playerChooseStuff(player *player);
void printChooseArmor(player *player, item **listArmors);
void printChooseWeapon(player *player, item **listWeapons);


int playerTurnFight(player *player, monster *monster, uint16_t monsterMaxLife);
int playerDoDamage(player *player, monster *monster);
int playerSwitchWeapon(player *player);
int playerSwitchArmor(player *player);
int playerUsePotion(player *player);
int playerEscape(player *player);
int playerDoChoiceCategory(item **tabItem);

//---------------------- Map ----------------------
void playerMove(player *player, level *map, direction direction);



//---------------------- Affichage ----------------------
void displayPlayerOnMap(player *p, level *map);
void printPlayer(player *player);
void printLevel(player *player);
void printExp(player *player);
void printLife(uint16_t current, uint16_t max);
void printBar(float value, float max, int size);

void playerInterfaceFight();
void printMonster(monster *monster, uint16_t monsterMaxLife);
void printFightIcon();

#endif //MALLOCWORLD_PLAYER_H
