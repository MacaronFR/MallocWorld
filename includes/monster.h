#ifndef MALLOCWORLD_MONSTER_H
#define MALLOCWORLD_MONSTER_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <dirent.h>

#include <terminalManager.h>
#include <utils.h>


typedef struct s_monster{
	uint16_t life;
	uint8_t respawn;
	uint16_t strength;
	char *name;
	uint32_t id;
	int16_t exp;
	uint8_t zone;
} monster;

monster *createMonster(monster *m);
void freeMonster(monster *m);
monster *loadMonster(const char *filename);
monster **loadMonsters(const char *dir, size_t *nMonster);
void freeMonsterList(monster **monsterList, size_t nMonster);
monster *findMonster(monster **monsterList, size_t nMonster, int32_t id);

void monsterTakeDamage(monster *monster, int amount);
void monsterIncrementTime(monster *monster);

#endif //MALLOCWORLD_MONSTER_H
