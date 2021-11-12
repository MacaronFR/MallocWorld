#ifndef MALLOCWORLD_MONSTER_H
#define MALLOCWORLD_MONSTER_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <utils.h>
#include <player.h>

typedef struct s_monster{
	u_int16_t life;
	u_int8_t respawn;
	u_int16_t strength;
	char *name;
	u_int32_t id;
} monster;

monster *createMonster(monster *m);
void deleteMonster(monster *m);
monster *loadMonster(const char *filename);


void monsterTakeDamage(monster *monster, int amount);
void monsterAttack(monster *monster, player *player);
void monsterIncrementTime(monster *monster);

#endif //MALLOCWORLD_MONSTER_H
